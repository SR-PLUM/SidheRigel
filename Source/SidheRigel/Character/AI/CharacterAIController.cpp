// Fill out your copyright notice in the Description page of Project Settings.


#include "CharacterAIController.h"
#include "SidheRigel/Character/AI/AISidheRigelCharacter.h"
#include "SidheRigel/Minion/WayPoint.h"

#include "Kismet/GameplayStatics.h"

void ACharacterAIController::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	if (AIPlayer)
	{
		
	}
}

void ACharacterAIController::BeginPlay()
{
	Super::BeginPlay();

	UGameplayStatics::GetAllActorsOfClass(GetWorld(), AWayPoint::StaticClass(), WayPoints);

	currentWayPointOrder = WayPoints.Num() - 2;
}

void ACharacterAIController::OnPossess(APawn* InPawn)
{
	Super::OnPossess(InPawn);

	UE_LOG(LogTemp, Warning, TEXT("Possesed in %s"), *InPawn->GetName());
	AIPlayer = Cast<AAISidheRigelCharacter>(InPawn);
	if (AIPlayer)
	{
		UE_LOG(LogTemp, Warning, TEXT("Set AIController"));
		AIPlayer->AIController = this;
	}
}

void ACharacterAIController::OnUnPossess()
{
	Super::OnUnPossess();

	UE_LOG(LogTemp,Warning,TEXT("UnPossess"))
}

void ACharacterAIController::OnMoveCompleted(FAIRequestID RequestID, const FPathFollowingResult& Result)
{
	Super::OnMoveCompleted(RequestID, Result);

}

void ACharacterAIController::MoveToWayPoint()
{
	for (auto wayPoint : WayPoints)
	{
		AWayPoint* wayPointItr = Cast<AWayPoint>(wayPoint);

		if (wayPointItr)
		{
			if (wayPointItr->wayPointOrder == currentWayPointOrder)
			{
				currentWayPoint = wayPointItr;
				MoveToActor(wayPointItr);
			}
		}
	}
}
