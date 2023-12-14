// Fill out your copyright notice in the Description page of Project Settings.


#include "AISidheRigelCharacter.h"

#include "Components/SphereComponent.h"
#include "Kismet/GameplayStatics.h"
#include "Blueprint/AIBlueprintHelperLibrary.h"

#include "CharacterAIController.h"
#include "SidheRigel/Minion/WayPoint.h"
#include "SidheRigel/Minion/Minion.h"
#include "SidheRIgel/Tower/Tower.h"
#include "SidheRigel/Nexus/Nexus.h"
#include "SidheRigel/UI/StatSummary.h"

AAISidheRigelCharacter::AAISidheRigelCharacter()
{
	AIControllerClass = ACharacterAIController::StaticClass();
	AutoPossessAI = EAutoPossessAI::PlacedInWorldOrSpawned;

	static ConstructorHelpers::FObjectFinder<UBlueprint> deathActorRef(TEXT("/Game/Heros/AI/BP_KerunAIDeathActor"));
	if (deathActorRef.Object)
	{
		deathActorClass = (UClass*)deathActorRef.Object->GeneratedClass;
	}
}

void AAISidheRigelCharacter::BeginPlay()
{
	Super::BeginPlay();

	UGameplayStatics::GetAllActorsOfClass(GetWorld(), AWayPoint::StaticClass(), WayPoints);

	currentWayPointOrder = 5;

	if (AIController)
	{
		currentWayPoint = GetWayPoint(currentWayPointOrder);
		AIController->MoveToActor(currentWayPoint);
	}

	IsSelectedTalent[0].IsSelected[2] = true;

	spawnLocation = GetActorLocation();
}

void AAISidheRigelCharacter::Tick(float DeltaSeconds)
{
	Super::Tick(DeltaSeconds);
	if (!AIController)
	{
		if (!GetController())
		{
			SpawnDefaultController();
		}
	}

	if (DieTime <= 0 && isDie)
	{
		UE_LOG(LogTemp, Warning, TEXT("RERIVE"));

		GetMesh()->SetVisibility(true);
		SetCurrentHP(GetMaxHP());
		TurnOnStatUI();

		isDie = false;
	}
	else if (DieTime > 0)
	{
		return;
	}

	if (currentAttackDelay > 0)
	{
		currentAttackDelay -= DeltaSeconds;
	}

	//근처에 타워가 있으면
	if (IsNearByTower())
	{
		AActor* towerTarget = nearbyTower->currentTarget;
		if (towerTarget != nullptr)
		{
			if (towerTarget == this)
			{
				MoveBackward();
				return;
			}
			else if (IsTargetHero())
			{
				currentTarget = GetClosestEnemy();
				if (IsTargetHero())
				{
					currentTarget = nullptr;
					MoveBackward();
				}
			}
		}
		else
		{
			MoveBackward();
			return;
		}
	}

	//근처 아군의 수가 1이하일때 후퇴
	if (!HasNearByTeam())
	{
		MoveBackward();
		return;
	}

	//공격 시도
	if (HasTarget())
	{
		ChangeTargetIfTargetDead();

		if (!IsEnoughCloseToTarget())
		{
			if (IsTargetHero())
			{
				ApproachToAttackUsingSkill();
			}

			if (AIController)
			{
				AIController->MoveToActor(currentTarget);
			}

			return;
		}
		
		//공격 할 수 있으면 공격
		if (currentAttackDelay <= 0)
		{
			//currentTarget이 적 영웅이면 Q스킬 사용
			if (IsTargetHero())
			{
				UsingQSkill();
			}

			AIAttack();

			return;
		}
	}
	
	//Move
	if (currentWayPoint)
	{
		MoveForward();
	}
	else
	{
		currentWayPoint = GetWayPoint(currentWayPointOrder);
	}
}

void AAISidheRigelCharacter::SetTeam(E_Team setTeam)
{
	team = setTeam;
}

void AAISidheRigelCharacter::OnEnterEnemy(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{
	if (ITeam* TeamEnemy = Cast<ITeam>(OtherActor))
	{
		if (TeamEnemy->GetTeam() != GetTeam())
		{
			if (AMinion* MinionEnemy = Cast<AMinion>(OtherActor))
			{
				InRangeActors.Add(OtherActor);
			}
			else if (ATower* TowerEnemy = Cast<ATower>(OtherActor))
			{
				InRangeActors.Add(OtherActor);
			}
			else if (ANexus* NexusEnemy = Cast<ANexus>(OtherActor))
			{
				InRangeActors.Add(OtherActor);
			}
			else if (ASidheRigelCharacter* SRCharacter = Cast<ASidheRigelCharacter>(OtherActor))
			{
				InRangeActors.Add(OtherActor);
			}
			if (currentTarget == nullptr)
			{
				currentTarget = OtherActor;
			}
		}
		else
		{
			TeamInRange.Add(OtherActor);
		}
	}
}

void AAISidheRigelCharacter::OnExitEnemy(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex)
{
	Super::OnExitEnemy(OverlappedComponent, OtherActor, OtherComp, OtherBodyIndex);

	TeamInRange.Remove(OtherActor);

	if (currentTarget == OtherActor)
	{
		if (InRangeActors.IsEmpty())
		{
			currentTarget = nullptr;
		}
		else
		{
			//가장 가까운 적 대상
			float maxDistance = 0;
			for (auto enemy : InRangeActors)
			{
				auto enemyDist = GetDistanceTo(enemy);

				if (maxDistance < enemyDist)
				{
					maxDistance = enemyDist;
					currentTarget = enemy;
				}
			}
		}
	}
}

AWayPoint* AAISidheRigelCharacter::GetWayPoint(int idx)
{
	AWayPoint* getWayPoint = nullptr;

	for (auto wayPoint : WayPoints)
	{
		AWayPoint* wayPointItr = Cast<AWayPoint>(wayPoint);

		if (wayPointItr)
		{
			if (wayPointItr->wayPointOrder == idx)
			{
				getWayPoint = wayPointItr;
			}
		}
	}
	return getWayPoint;
}

void AAISidheRigelCharacter::GiveExp(int32 _exp)
{
	experience += _exp;
	if (experience >= MaxExperience)
	{
		experience -= MaxExperience;
		level++;
		int32 idx = 0;
		if (level == 4)
		{
			IsSelectedTalent[1].IsSelected[2] = true;
		}
		if (level == 7)
		{
			IsSelectedTalent[2].IsSelected[1] = true;
		}
		if (level == 10)
		{
			IsSelectedTalent[3].IsSelected[0] = true;
		}
		if (level == 13)
		{
			IsSelectedTalent[4].IsSelected[0] = true;
		}
		if (level == 16)
		{
			IsSelectedTalent[5].IsSelected[0] = true;
		}
		if (level == 20)
		{
			IsSelectedTalent[6].IsSelected[1] = true;
		}
	}

	StatSummary->SetLevel(level);
	StatSummary->SetExpBar(float(experience) / MaxExperience);
}

void AAISidheRigelCharacter::TakeDamage(float damage, AActor* damageCauser)
{
	Super::TakeDamage(damage, damageCauser);

	if (isDie)
	{
		GetMesh()->SetVisibility(false);
		SetActorLocation(spawnLocation);
		TurnOffStatUI();
	}
}

bool AAISidheRigelCharacter::IsNearByTower()
{
	nearbyTower = nullptr;
	for (auto actor : InRangeActors)
	{
		if (auto teamActor = Cast<ITeam>(actor))
		{
			if (teamActor->GetTeam() != GetTeam())
			{
				if (auto towerActor = Cast<ATower>(actor))
				{
					nearbyTower = towerActor;
					return true;
				}
			}
		}
	}
	return false;
}

bool AAISidheRigelCharacter::HasTarget()
{
	if (currentTarget)
	{
		//if (GetDistanceTo(currentTarget) > 500)
		//{
		//	auto closeActor = GetClosestEnemy();
		//	if (GetDistanceTo(closeActor) > 500)
		//	{
		//		return false;
		//	}
		//	else
		//	{
		//		currentTarget = closeActor;

		//		return true;
		//	}
		//}
		
		return true;
	}
	else
		return false;
}

bool AAISidheRigelCharacter::HasNearByTeam()
{
	int32 teamCount = 0;

	if (TeamInRange.IsEmpty())
	{
		teamCount = 0;
	}
	else
	{
		for (auto teamActor : TeamInRange)
		{
			if (auto minionActor = Cast<AMinion>(teamActor))
			{
				teamCount = TeamInRange.Num();
			}
		}
	}
	
	if (teamCount < 2)
		return false;
	else
		return true;
}

void AAISidheRigelCharacter::MoveBackward()
{
	if (GetDistanceTo(GetWayPoint(currentWayPointOrder + 1)) <= 150)
	{
		if (currentWayPointOrder < 5)
		{
			currentWayPointOrder += 1;
			currentWayPoint = GetWayPoint(currentWayPointOrder);
		}
	}
	
	if (AIController)
	{
		AIController->MoveToActor(GetWayPoint(currentWayPointOrder + 1));
	}
}

bool AAISidheRigelCharacter::IsEnoughCloseToTarget()
{
	if (GetDistanceTo(currentTarget) > GetRange())
	{
		return false;
	}
	return true;
}

bool AAISidheRigelCharacter::IsTargetHero()
{
	if (auto SREnemy = Cast<ASidheRigelCharacter>(currentTarget))
	{
		return true;
	}
	return false;
}

void AAISidheRigelCharacter::ApproachToAttackUsingSkill()
{
	auto SREnemy = Cast<ASidheRigelCharacter>(currentTarget);

	//대상의 현재 체력이 최대체력의 30% 이하라면
	if (SREnemy->GetHP() < SREnemy->GetMaxHP() * 0.3)
	{
		//W스킬 사용전 마나 및 쿨타임 확인
		if (skills[E_SkillState::W_Ready]->GetCooldown() <= 0)
		{
			if (skills[E_SkillState::W_Ready]->CanUse())
			{
				FHitResult Hit = FHitResult(SREnemy, nullptr, FVector(), FVector());
				skills[E_SkillState::W_Ready]->OnUse(Hit);
				skills[E_SkillState::W_Ready]->SetCooldown();
			}
		}

		//R스킬 사용전 마나 및 쿨타임 확인
		if (skills[E_SkillState::R_Ready]->GetCooldown() <= 0)
		{
			if (skills[E_SkillState::R_Ready]->CanUse())
			{
				FHitResult Hit = FHitResult(SREnemy, nullptr, FVector(), FVector());
				skills[E_SkillState::R_Ready]->OnUse(Hit);
				skills[E_SkillState::R_Ready]->SetCooldown();
			}
		}
	}
}

void AAISidheRigelCharacter::UsingQSkill()
{
	//Q스킬 사용전 마나 및 쿨타임 확인
	if (skills[E_SkillState::Q_Ready]->GetCooldown() <= 0)
	{
		if (skills[E_SkillState::Q_Ready]->CanUse())
		{
			FHitResult Hit;
			skills[E_SkillState::Q_Ready]->OnUse(Hit);
			skills[E_SkillState::Q_Ready]->SetCooldown();
		}
	}
}

void AAISidheRigelCharacter::AIAttack()
{
	Attack(currentTarget);
	//공격속도 적용
	currentAttackDelay = 1 / GetAttackSpeed();
}

void AAISidheRigelCharacter::ChangeTargetIfTargetDead()
{
	if (auto damagableActor = Cast<IDamagable>(currentTarget))
	{
		//적이 죽었으면 대상변경
		if (damagableActor->GetHP() <= 0)
		{
			InRangeActors.Remove(currentTarget);

			currentTarget = GetClosestEnemy();
		}
	}
}

AActor* AAISidheRigelCharacter::GetClosestEnemy()
{
	AActor* ClosestEnemy = nullptr;
	if (!InRangeActors.IsEmpty())
	{
		//가장 가까운 적 대상
		float maxDistance = 0;
		for (auto enemy : InRangeActors)
		{
			if (auto teamEnemy = Cast<ITeam>(enemy))
			{
				if (teamEnemy->GetTeam() != GetTeam())
				{
					auto enemyDist = GetDistanceTo(enemy);

					if (maxDistance < enemyDist)
					{
						maxDistance = enemyDist;
						ClosestEnemy = enemy;
					}
				}
			}
		}
	}
	else //더이상 적이 없으면 currentTarget == nullprt
	{
		ClosestEnemy = nullptr;
	}

	return ClosestEnemy;
}

void AAISidheRigelCharacter::MoveForward()
{
	if (GetDistanceTo(currentWayPoint) <= 150.f)
	{
		if (GetWayPoint(currentWayPointOrder - 1)->currentRedMinion > 2)
		{
			currentWayPointOrder -= 1;
			currentWayPoint = GetWayPoint(currentWayPointOrder);
		}
	}
	
	if (AIController)
	{
		AIController->MoveToActor(currentWayPoint);
	}
}

