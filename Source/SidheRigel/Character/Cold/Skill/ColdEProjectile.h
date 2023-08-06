// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "ColdEProjectile.generated.h"

UCLASS()
class SIDHERIGEL_API AColdEProjectile : public AActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	AColdEProjectile();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

private:
	UPROPERTY(VisibleAnywhere)
		class USphereComponent* CollisionComponent;

	UPROPERTY(VisibleAnywhere)
		class UProjectileMovementComponent* ProjectileMovementComponent;

	UPROPERTY(VisibleAnywhere)
		class UStaticMeshComponent* ProjectileMesh;

private:
	UPROPERTY()
		FVector start;
	UPROPERTY()
		FVector destination;
	UPROPERTY()
		FVector direction;
	UPROPERTY()
		double totalLength;

	UPROPERTY()
		bool isReadyToFire = false;

public:
	UPROPERTY()
		AActor* projectileOwner;

	UFUNCTION()
		void SetLocation(FVector location);
};
