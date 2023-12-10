// Fill out your copyright notice in the Description page of Project Settings.


#include "AISidheRigelCharacter.h"

#include "Components/SphereComponent.h"
#include "Kismet/GameplayStatics.h"
#include "Blueprint/AIBlueprintHelperLibrary.h"

#include "CharacterAIController.h"
#include "SidheRigel/Minion/WayPoint.h"

AAISidheRigelCharacter::AAISidheRigelCharacter()
{
	AIControllerClass = ACharacterAIController::StaticClass();
	AutoPossessAI = EAutoPossessAI::PlacedInWorldOrSpawned;
}

void AAISidheRigelCharacter::BeginPlay()
{
	Super::BeginPlay();

	UGameplayStatics::GetAllActorsOfClass(GetWorld(), AWayPoint::StaticClass(), WayPoints);

	currentWayPointOrder = WayPoints.Num() - 2;

	MoveToWayPoint(currentWayPointOrder);
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

	if (currentWayPoint)
	{
		if (WayPoints.Num() > 0)
		{
			if (GetDistanceTo(currentWayPoint) <= 150.f)
			{
				currentWayPointOrder--;

				MoveToWayPoint(currentWayPointOrder);
			}
			else
			{
				MoveToWayPoint(currentWayPointOrder);
			}
		}
	}
	else
	{
		MoveToWayPoint(currentWayPointOrder);
	}

	//Move
	//공격 대상이 있다면
	if (currentTarget)
	{
		//상대가 멀다면 상대 방향으로 이동
		if (GetDistanceTo(currentTarget) > GetRange())
		{
			if (AIController)
			{
				//UE_LOG(LogTemp, Warning, TEXT("MoveToActor"));
				AIController->MoveToActor(currentTarget);
				//UAIBlueprintHelperLibrary::SimpleMoveToLocation(AIController, currentTarget->GetActorLocation());
			}
		}
		else
		{
			if (AIController)
			{
				//UE_LOG(LogTemp, Warning, TEXT("StopMovement"));
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
	Super::OnEnterEnemy(OverlappedComponent, OtherActor, OtherComp, OtherBodyIndex, bFromSweep, SweepResult);

	//UE_LOG(LogTemp, Warning, TEXT("Enter Enemy"))

	if (ITeam* TeamEnemy = Cast<ITeam>(OtherActor))
	{
		if (TeamEnemy->GetTeam() != GetTeam())
		{
			if (ASidheRigelCharacter* SRCharacter = Cast<ASidheRigelCharacter>(OtherActor))
			{
				InRangeActors.Add(OtherActor);
				
				if (currentTarget == nullptr)
				{
					currentTarget = OtherActor;
				}
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

void AAISidheRigelCharacter::MoveToWayPoint(int wayPointIndex)
{
	if (AIController)
	{
		for (auto wayPoint : WayPoints)
		{
			AWayPoint* wayPointItr = Cast<AWayPoint>(wayPoint);

			if (wayPointItr)
			{
				if (wayPointItr->wayPointOrder == wayPointIndex)
				{
					currentWayPoint = wayPointItr;
					AIController->MoveToActor(wayPointItr);
				}
			}
		}
	}
}


