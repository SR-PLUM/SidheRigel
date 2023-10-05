// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "MinionProjectile.generated.h"

UCLASS()
class SIDHERIGEL_API AMinionProjectile : public AActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	AMinionProjectile();

protected:
	UPROPERTY(VisibleAnywhere)
		class UProjectileMovementComponent* ProjectileMovementComponent;
	UPROPERTY(VisibleAnywhere)
		class UStaticMeshComponent* ProjectileMesh;

public:
	UPROPERTY()
		AActor* target;
	UPROPERTY()
		float attackDamage;
	UPROPERTY()
		AActor* projectileOwner;


protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

};
