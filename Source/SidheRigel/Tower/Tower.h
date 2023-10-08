// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"

#include "../Interface/Damagable.h"
#include "../Interface/Team.h"

#include "Tower.generated.h"

UCLASS()
class SIDHERIGEL_API ATower : public AActor, public IDamagable, public ITeam
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	ATower();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

protected:
	class USphereComponent* rangeArea;

	UPROPERTY(VisibleAnywhere)
		class UStaticMeshComponent* mesh;

	UPROPERTY(EditAnywhere)
		TEnumAsByte<E_Team> team = E_Team::Team_Null;

	float HP = 5000;
public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	UFUNCTION()
		void OnEnterEnemy(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult);
	UFUNCTION()
		void OnExitEnemy(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex);

public:
	TArray<class AActor*> attackList;

	AActor* currentTarget;

	float attackDelay = 0.f;
	float maxAttackDelay = 1.5f;

	float damage = 180;
public:
	UFUNCTION()
		virtual void TakeDamage(float damage, AActor* damageCauser);
	UFUNCTION()
		virtual void RestoreHP(float value);
	UFUNCTION()
		virtual float GetHP();

	UFUNCTION()
		virtual E_Team GetTeam();
};
