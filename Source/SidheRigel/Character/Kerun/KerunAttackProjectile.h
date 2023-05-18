// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "../../Dummy/DummyProjectile.h"
#include "KerunAttackProjectile.generated.h"

UCLASS()
class SIDHERIGEL_API AKerunAttackProjectile : public ADummyProjectile
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	AKerunAttackProjectile();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;


protected:	//InitFunction
	virtual void SetProjectileMesh() override;
	virtual void SetProjectileMovementComponent() override;
	/*
public:
	UPROPERTY()
		AActor* Target;
	UPROPERTY()
		float AttackDamage;
	UPROPERTY()
		float criticalRate;
	UPROPERTY()
		float criticalDamage;

private:
	UPROPERTY(VisibleAnywhere)
		class USphereComponent* CollisionComponent;

	UPROPERTY(VisibleAnywhere)
		class UProjectileMovementComponent* ProjectileMovementComponent;
		*/
};
