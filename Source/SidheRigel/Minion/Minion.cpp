// Fill out your copyright notice in the Description page of Project Settings.


#include "Minion.h"

#include "MinionAIController.h"
#include "WayPoint.h"

#include "Kismet/GameplayStatics.h"
#include "Components/SphereComponent.h"
#include "GameFramework/CharacterMovementComponent.h"

// Sets default values
AMinion::AMinion()
{
 	// Set this character to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	if (!detectArea)
	{
		detectArea = CreateDefaultSubobject<USphereComponent>(TEXT("detectArea"));
		detectArea->InitSphereRadius(500.0f);
		detectArea->SetupAttachment(RootComponent);
	}

	GetCharacterMovement()->MaxWalkSpeed = 400.f;
}

// Called when the game starts or when spawned
void AMinion::BeginPlay()
{
	Super::BeginPlay();
	
	AIControllerClass = AMinionAIController::StaticClass();
	AutoPossessAI = EAutoPossessAI::PlacedInWorldOrSpawned;

	UGameplayStatics::GetAllActorsOfClass(GetWorld(), AWayPoint::StaticClass(), WayPoints);

	detectArea->OnComponentBeginOverlap.AddDynamic(this, &AMinion::OnEnterEnemy);
	detectArea->OnComponentEndOverlap.AddDynamic(this, &AMinion::OnExitEnemy);

	AIController = Cast<AAIController>(GetController());

	MoveToWayPoint();
}

// Called every frame
void AMinion::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	//If Goal WayPoint Move To Next WayPoint
	if (currentWayPoint)
	{
		if (currentWayPointOrder < WayPoints.Num())
		{
			if (GetDistanceTo(currentWayPoint) <= 100.f)
			{
				currentWayPointOrder++;
				MoveToWayPoint();
			}
		}
	}

	if (attackDelay > 0)
	{
		attackDelay -= DeltaTime;
	}

	if (currentTarget)
	{
		IDamagable* damagableTarget = Cast<IDamagable>(currentTarget);
		if (damagableTarget)
		{
			if (damagableTarget->GetHP() <= 0)
			{
				attackList.Remove(currentTarget);

				if (attackList.Num() == 0)
				{
					currentTarget = nullptr;
				}
				else
				{
					currentTarget = attackList.Top();
				}
			}
			else if (GetDistanceTo(currentTarget) <= range && attackDelay <= 0)
			{
				damagableTarget->TakeDamage(damage, this);

				attackDelay = maxAttackDelay;
			}
			else
			{
				AIController->MoveToActor(currentTarget, range);
			}
		}

		
	}
}

// Called to bind functionality to input
void AMinion::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);

}

void AMinion::MoveToWayPoint()
{
	if (AIController)
	{
		for (auto wayPoint : WayPoints)
		{
			AWayPoint* wayPointItr = Cast<AWayPoint>(wayPoint);
			
			if (wayPointItr)
			{
				if (wayPointItr->wayPointOrder == currentWayPointOrder)
				{
					currentWayPoint = wayPointItr;
					AIController->MoveToActor(wayPointItr);
				}
			}
		}
	}
}

void AMinion::OnEnterEnemy(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{
	if (ITeam* Enemy = Cast<ITeam>(OtherActor))
	{
		if (Enemy->GetTeam() == E_Team::Blue)
		{
			if (IDamagable* DamagableActor = Cast<IDamagable>(OtherActor))
			{
				AIController->StopMovement();

				if (attackList.Num() == 0)
				{
					currentTarget = OtherActor;
				}

				attackList.Add(OtherActor);
				UE_LOG(LogTemp, Warning, TEXT("ENTER ENEMY : %s"), *OtherActor->GetName());
			}
		}
	}
}

void AMinion::OnExitEnemy(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex)
{
	if (ITeam* Enemy = Cast<ITeam>(OtherActor))
	{
		if (IDamagable* DamagableActor = Cast<IDamagable>(OtherActor))
		{
			AIController->StopMovement();

			attackList.Remove(OtherActor);

			if (currentTarget == OtherActor)
			{
				if (attackList.IsEmpty())
				{
					currentTarget = nullptr;

					MoveToWayPoint();
				}
				else
				{
					currentTarget = attackList.Top();
				}
			}
			UE_LOG(LogTemp, Warning, TEXT("EXIT ENEMY : %s"), *OtherActor->GetName());
		}
	}
}

E_Team AMinion::GetTeam()
{
	return team;
}

void AMinion::Attack(AActor* Target)
{
}

void AMinion::Stun(float time)
{
}

void AMinion::Stop(float time)
{
}

void AMinion::Slow(float time, float value, FString key)
{
}

void AMinion::Silence(float time)
{
}

void AMinion::Airborne(float time)
{
}

void AMinion::TakeDamage(float _damage, AActor* damageCauser)
{
}

void AMinion::RestoreHP(float value)
{
}

float AMinion::GetHP()
{
	return 0.0f;
}

void AMinion::MoveVector(FVector Direction, float Force)
{
}
