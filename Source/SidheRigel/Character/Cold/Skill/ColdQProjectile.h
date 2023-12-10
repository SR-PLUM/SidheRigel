// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "ColdQProjectile.generated.h"

UCLASS()
class SIDHERIGEL_API AColdQProjectile : public AActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	AColdQProjectile();

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
		float slowRate;
	UPROPERTY()
		float slowTime;

private:
	UPROPERTY(VisibleAnywhere)
		class UProjectileMovementComponent* ProjectileMovementComponent;

	UPROPERTY(VisibleAnywhere)
		class UStaticMeshComponent* ProjectileMesh;

	TSubclassOf<class AColdQParticle> particleClass;
	TSubclassOf<class AColdQSplash> splashClass; 
public:
	UPROPERTY(EditAnywhere)
		class USoundBase* SpawnSound;
};
