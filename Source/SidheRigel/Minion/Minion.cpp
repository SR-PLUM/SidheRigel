// Fill out your copyright notice in the Description page of Project Settings.


#include "Minion.h"

#include "MinionAIController.h"
#include "WayPoint.h"

#include "Kismet/GameplayStatics.h"

// Sets default values
AMinion::AMinion()
{
 	// Set this character to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;


}

// Called when the game starts or when spawned
void AMinion::BeginPlay()
{
	Super::BeginPlay();
	
	AIControllerClass = AMinionAIController::StaticClass();
	AutoPossessAI = EAutoPossessAI::PlacedInWorldOrSpawned;

	UGameplayStatics::GetAllActorsOfClass(GetWorld(), AWayPoint::StaticClass(), WayPoints);

	MoveToWayPoint();
}

// Called every frame
void AMinion::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

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
					AIController->MoveToActor(wayPointItr);
				}
			}
		}
	}
}

