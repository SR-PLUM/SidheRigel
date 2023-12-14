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
		float damage;
	UPROPERTY()
		float duration = 1.f;
	UPROPERTY(VisibleAnywhere)
		class USplineMeshComponent* splineComponent;

private:
	TSubclassOf<class AColdR2Laser> laserClass;

};
