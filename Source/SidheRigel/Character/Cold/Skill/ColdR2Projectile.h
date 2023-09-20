// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "ColdR2Projectile.generated.h"

UCLASS()
class SIDHERIGEL_API AColdR2Projectile : public AActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	AColdR2Projectile();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

public:
	UPROPERTY()
		AActor* projectileOwner;
	UPROPERTY()
		FVector startLocation;
	UPROPERTY()
		FVector forwardVector;
	UPROPERTY()
		float range;
	UPROPERTY()
		float damage;

protected:
	UFUNCTION()
		void OnColliderOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult);

private:
	UPROPERTY(VisibleAnywhere)
		class USphereComponent* CollisionComponent;

	UPROPERTY(VisibleAnywhere)
		class UProjectileMovementComponent* ProjectileMovementComponent;

	UPROPERTY(VisibleAnywhere)
		class UStaticMeshComponent* ProjectileMesh;

};
