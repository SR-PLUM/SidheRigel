// Fill out your copyright notice in the Description page of Project Settings.


#include "AISidheRigelCharacter.h"

#include "Components/SphereComponent.h"
#include "Kismet/GameplayStatics.h"
#include "Blueprint/AIBlueprintHelperLibrary.h"

#include "CharacterAIController.h"
#include "SidheRigel/Minion/WayPoint.h"
#include "SidheRigel/Minion/Minion.h"
#include "SidheRIgel/Tower/Tower.h"

AAISidheRigelCharacter::AAISidheRigelCharacter()
{
	AIControllerClass = ACharacterAIController::StaticClass();
	AutoPossessAI = EAutoPossessAI::PlacedInWorldOrSpawned;
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

	//Attack
	if (currentTarget)
	{
		//진행중인 방향의 미니언이 없고 현재 위치에도 적은수의 미니언이 남았다면
		//수정필요
		if (GetWayPoint(currentWayPointOrder - 1)->currentRedMinion == 0 &&
			GetWayPoint(currentWayPointOrder)->currentRedMinion < 2)
		{
			//후진
			currentWayPointOrder = currentWayPointOrder + 1;
			currentWayPoint = GetWayPoint(currentWayPointOrder);
		}
		else
		{
			//상대가 멀다면 상대 방향으로 이동
			if (GetDistanceTo(currentTarget) > GetRange())
			{
				if (AIController)
				{
					//만약 대상이 적 영웅이고
					if (auto SREnemy = Cast<ASidheRigelCharacter>(currentTarget))
					{
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

									//R스킬 사용전 마나 및 쿨타임 확인
									if (skills[E_SkillState::R_Ready]->GetCooldown() <= 0)
									{
										if (skills[E_SkillState::R_Ready]->CanUse())
										{
											skills[E_SkillState::R_Ready]->OnUse(Hit);
											skills[E_SkillState::R_Ready]->SetCooldown();
										}
									}
								}
							}
						}
					}

					AIController->MoveToActor(currentTarget);
				}
			}
			else
			{
				if (AIController)
				{
					AIController->StopMovement();
				}
				//공격 할 수 있으면 공격
				if (currentAttackDelay <= 0)
				{
					//currentTarget이 적 영웅이면 Q스킬 사용
					if (auto SREnemy = Cast<ASidheRigelCharacter>(currentTarget))
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

					Attack(currentTarget);
					//공격속도 적용
					currentAttackDelay = 1 / GetAttackSpeed();

					if (auto damagableActor = Cast<IDamagable>(currentTarget))
					{
						//적이 죽었으면 대상변경
						if (damagableActor->GetHP() <= 0)
						{
							if (!InRangeActors.IsEmpty())
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
							else//더이상 적이 없으면 currentTarget == nullprt
							{
								currentTarget = nullptr;
							}
						}
					}
				}
			}
		}
	}
	else
	{
		//Move
		if (currentWayPoint)
		{
			//도착
			if (GetDistanceTo(currentWayPoint) <= 150.f)
			{
				//진행중인 방향의 미니언이 4마리 이상이라면
				if (GetWayPoint(currentWayPointOrder - 1)->currentRedMinion > 3)
				{
					UE_LOG(LogTemp, Warning, TEXT("Move Next WayPoint"))
					//전진
					currentWayPointOrder = currentWayPointOrder - 1;
					currentWayPoint = GetWayPoint(currentWayPointOrder);
				}
			}
			else
			{
				AIController->MoveToActor(GetWayPoint(currentWayPointOrder));
			}
		}
		else
		{
			currentWayPoint = GetWayPoint(currentWayPointOrder);
		}
	}

	if (currentAttackDelay > 0)
	{
		currentAttackDelay -= DeltaSeconds;
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
			else if (ASidheRigelCharacter* SRCharacter = Cast<ASidheRigelCharacter>(OtherActor))
			{
				InRangeActors.Add(OtherActor);
			}
			if (currentTarget == nullptr)
			{
				currentTarget = OtherActor;
			}
		}
	}
}

void AAISidheRigelCharacter::OnExitEnemy(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex)
{
	Super::OnExitEnemy(OverlappedComponent, OtherActor, OtherComp, OtherBodyIndex);

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
}