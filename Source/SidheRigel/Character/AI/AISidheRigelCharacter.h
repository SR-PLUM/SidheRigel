// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "SidheRigel/Character/Kerun/KerunCharacter.h"
#include "AISidheRigelCharacter.generated.h"

/**
 * 
 */
UCLASS()
class SIDHERIGEL_API AAISidheRigelCharacter : public AKerunCharacter
{
	GENERATED_BODY()
	
public:
	AAISidheRigelCharacter();

	virtual void BeginPlay() override;
	virtual void Tick(float DeltaSeconds) override;

	void SetTeam(E_Team setTeam);

	UPROPERTY(EditAnyWhere)
		class ACharacterAIController* AIController;

	UPROPERTY()
		AActor* currentTarget = nullptr;
	UPROPERTY()
		float currentAttackDelay = 0;

	void OnEnterEnemy(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult);
	void OnExitEnemy(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex);

	UPROPERTY(EditAnywhere)
		int32 currentWayPointOrder;
	UPROPERTY(EditAnywhere)
		class AWayPoint* currentWayPoint;
	UPROPERTY()
		TArray<AActor*> WayPoints;

	UFUNCTION()
		void MoveToWayPoint(int wayPointIndex);
};
