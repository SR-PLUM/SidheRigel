// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "AIController.h"
#include "CharacterAIController.generated.h"

/**
 * 
 */
UCLASS()
class SIDHERIGEL_API ACharacterAIController : public AAIController
{
	GENERATED_BODY()
	
public:
	class AAISidheRigelCharacter* AIPlayer;

	virtual void Tick(float DeltaTime) override;
	virtual void BeginPlay() override;

	virtual void OnPossess(APawn* InPawn) override;
	virtual void OnUnPossess() override;

	virtual void OnMoveCompleted(FAIRequestID RequestID, const FPathFollowingResult& Result) override;

	UFUNCTION()
		void MoveToWayPoint();
	UPROPERTY()
		TArray<AActor*> WayPoints;
	UPROPERTY(EditAnywhere)
		int32 currentWayPointOrder;
	UPROPERTY()
		class AWayPoint* currentWayPoint;
};
