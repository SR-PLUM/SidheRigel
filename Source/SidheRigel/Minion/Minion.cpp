// Fill out your copyright notice in the Description page of Project Settings.


#include "Minion.h"

#include "MinionAIController.h"
#include "WayPoint.h"

#include "Kismet/GameplayStatics.h"
#include "Components/SphereComponent.h"

// Sets default values
AMinion::AMinion()
{
 	// Set this character to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	if (!sphereComponent)
	{
		sphereComponent = CreateDefaultSubobject<USphereComponent>(TEXT("SphereComponent"));
		sphereComponent->InitSphereRadius(500.0f);
		sphereComponent->SetupAttachment(RootComponent);
	}
}

// Called when the game starts or when spawned
void AMinion::BeginPlay()
{
	Super::BeginPlay();
	
	AIControllerClass = AMinionAIController::StaticClass();
	AutoPossessAI = EAutoPossessAI::PlacedInWorldOrSpawned;

	UGameplayStatics::GetAllActorsOfClass(GetWorld(), AWayPoint::StaticClass(), WayPoints);

	sphereComponent->OnComponentBeginOverlap.AddDynamic(this, &AMinion::OnEnterEnemy);
	sphereComponent->OnComponentEndOverlap.AddDynamic(this, &AMinion::OnExitEnemy);

	MoveToWayPoint();
}

// Called every frame
void AMinion::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	//If Goal WayPoint Move To Next WayPoint
	/*if (currentWayPoint)
	{
		if (currentWayPointOrder < WayPoints.Num())
		{
			if (GetDistanceTo(currentWayPoint) <= 100.f)
			{
				currentWayPointOrder++;
				MoveToWayPoint();
			}
		}
	}*/
}

// Called to bind functionality to input
void AMinion::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);

}

void AMinion::MoveToWayPoint()
{
	auto AIController = Cast<AAIController>(GetController());

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
					AIController->MoveToActor(wayPointItr, 100.f);
				}
			}
		}
	}
}

void AMinion::OnEnterEnemy(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{
	if (IDamagable* DamagableActor = Cast<IDamagable>(OtherActor))
	{
		attackList.Add(DamagableActor);
	}
}

void AMinion::OnExitEnemy(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex)
{
	if (IDamagable* DamagableActor = Cast<IDamagable>(OtherActor))
	{
		attackList.Remove(DamagableActor);
	}
}

E_Team AMinion::GetTeam()
{
	return team;
}

