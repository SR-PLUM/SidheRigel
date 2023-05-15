// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "DummyProjectile.generated.h"

UCLASS()
class SIDHERIGEL_API ADummyProjectile : public AActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	ADummyProjectile();

	virtual void Tick(float DeltaTime) override;
	
protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:
	UPROPERTY()
		AActor* Target;
	UPROPERTY()
		float AttackDamage;
	UPROPERTY()
		float criticalRate;
	UPROPERTY()
		float criticalDamage;

protected:
	UPROPERTY(VisibleAnywhere)
		class USphereComponent* CollisionComponent;
	UPROPERTY(VisibleAnywhere)
		class UProjectileMovementComponent* ProjectileMovementComponent;
	UPROPERTY(VisibleAnywhere)
		class UStaticMeshComponent* ProjectileMesh;

protected:	//InitFunction
	virtual void SetProjectileMesh();
	virtual void SetProjectileMovementComponent();
};
