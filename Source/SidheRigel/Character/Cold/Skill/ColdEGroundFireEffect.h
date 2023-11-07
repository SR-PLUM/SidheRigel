// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "ColdEGroundFireEffect.generated.h"

UCLASS()
class SIDHERIGEL_API AColdEGroundFireEffect : public AActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	AColdEGroundFireEffect();
	
private:
	UPROPERTY(VisibleAnywhere)
		class USphereComponent* CollisionComponent;

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	float damageArea = 100.f;
	AActor* projectileOwner;
	float damage = 15.f;

	float elapsedTime = 0;
	float damageCycle = 0.2f;
	float duration = 1.5f;
};
