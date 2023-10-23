// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Character.h"
#include "../Interface/Team.h"
#include "../Interface/Damagable.h"
#include "../Interface/Attackable.h"
#include "../Interface/CCable.h"
#include "../Interface/Movable.h"

#include "Minion.generated.h"

UCLASS()
class SIDHERIGEL_API AMinion : public ACharacter, public IDamagable, public IAttackable, public ICCable, public IMovable, public ITeam
{
	GENERATED_BODY()

public:
	// Sets default values for this character's properties
	AMinion();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

protected:
	class USphereComponent* detectArea;

	class AMinionAIController* AIController;

	//DEBUG RED=MINION, BLUE = PLAYER
	UPROPERTY(EditAnywhere)
		TEnumAsByte<E_Team> team = E_Team::Red;

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
	//MOVE
	UPROPERTY(EditAnywhere)
		int32 currentWayPointOrder;

	UPROPERTY()
		class AWayPoint* currentWayPoint;

	UPROPERTY()
		TArray<AActor*> WayPoints;

	//ATTACK
	TSubclassOf<class AMinionProjectile> projectileClass;

	UPROPERTY()
		TArray<class AActor*> attackList;

	UPROPERTY()
		AActor* currentTarget;

	UPROPERTY()
		float range = 250.f;
	UPROPERTY()
		float projectileDamage = 10.f;

	UPROPERTY()
		float attackDelay = 0.f;
	UPROPERTY()
		float maxAttackDelay = 1.5f;

	UPROPERTY()
		float hp = 477;
	UPROPERTY()
		float maxHp = 477;

	UPROPERTY()
		int32 gold = 20;
	UPROPERTY()
		int32 exp = 60;

	bool isDie = false;

	UPROPERTY()
		bool IsAttackAnim = false;
	UPROPERTY()
		bool IsStun = false;
	UPROPERTY()
		FTimerHandle CheckStunTimer;
	UPROPERTY()
		uint32 StopRequestID;

protected: //UI
	UPROPERTY()
		class UWidgetComponent* MinionWidget;
	UPROPERTY()
		class UHPUI* MinionUIRef;

	void InitMinionWidget();
	void InitMinionUI();

protected:	//Move
	bool IsMoveVectorTrue = false;
	FVector moveDirection = FVector::ZeroVector;
	float moveForce = 0;
	int32 moveCnt = 0;

public:
	UFUNCTION()
		virtual void Attack(AActor* Target);

	UFUNCTION()
		virtual void Stun(float time);
	UFUNCTION()
		virtual void Stop(float time);
	UFUNCTION()
		virtual void Slow(float time, float value, FString key);
	UFUNCTION()
		virtual void Silence(float time);

	UFUNCTION()
		virtual void TakeDamage(float damage, AActor* damageCauser);
	UFUNCTION()
		virtual void RestoreHP(float value);
	UFUNCTION()
		virtual float GetHP();
		
	UFUNCTION()
		virtual void MoveVector(FVector Direction, float Force);

	UFUNCTION()
		virtual E_Team GetTeam() override;
	UFUNCTION()
		void SetTeam(E_Team _team);

protected:
	UPROPERTY()
		TMap<FString, float> speedRate;
};
