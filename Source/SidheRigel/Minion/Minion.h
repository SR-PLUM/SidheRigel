// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Character.h"
#include "../Interface/Team.h"

#include "Minion.generated.h"

UCLASS()
class SIDHERIGEL_API AMinion : public ACharacter, public ITeam
{
	GENERATED_BODY()

public:
	// Sets default values for this character's properties
	AMinion();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

protected:
	class USphereComponent* sphereComponent;

	//DEBUG RED=MINION, BLUE = PLAYER
	E_Team team = E_Team::Red;

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	// Called to bind functionality to input
	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;

	void MoveToWayPoint();

	UFUNCTION()
		void OnEnterEnemy(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult);
	UFUNCTION()
		void OnExitEnemy(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex);

public:
	UPROPERTY(EditAnywhere)
		int32 currentWayPointOrder;

	class AWayPoint* currentWayPoint;

	TArray<AActor*> WayPoints;

	TArray<class IDamagable*> attackList;

	virtual E_Team GetTeam() override;
};
