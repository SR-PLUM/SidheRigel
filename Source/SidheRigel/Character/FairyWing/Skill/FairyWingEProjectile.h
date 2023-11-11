// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "FairyWingEProjectile.generated.h"

UCLASS()
class SIDHERIGEL_API AFairyWingEProjectile : public AActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	AFairyWingEProjectile();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

public:
	UPROPERTY()
		AActor* target;
	UPROPERTY()
		AActor* projectileOwner;
	UPROPERTY()
		float damage;
	UPROPERTY()
		float speed;
	UPROPERTY()
		float slowValue;
	UPROPERTY()
		float slowTime;

private:
	UPROPERTY(EditAnywhere)
		class USphereComponent* CollisionComponent;

	UPROPERTY(EditAnywhere)
		class UProjectileMovementComponent* ProjectileMovementComponent;

	UPROPERTY(EditAnywhere)
		class UStaticMeshComponent* ProjectileMesh;

public:
	bool isHerohit;
};
