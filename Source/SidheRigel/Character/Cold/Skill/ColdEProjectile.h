// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "ColdEProjectile.generated.h"

UCLASS()
class SIDHERIGEL_API AColdEProjectile : public AActor
{
	GENERATED_BODY()

	const float ARC_VALUE = 0.2f;
	
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
		class UProjectileMovementComponent* ProjectileMovementComponent;

	UPROPERTY(VisibleAnywhere)
		class UStaticMeshComponent* ProjectileMesh;

private:

public:
	UPROPERTY()
		AActor* projectileOwner;

	UFUNCTION()
		void Launch(FVector startLoc, FVector targetLoc);

	class AColdEDamageField* damageField;

public:
	UPROPERTY(EditAnywhere)
		class USoundBase* SpawnSound;
};
