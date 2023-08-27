// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "ACMQCollider.generated.h"

UCLASS()
class SIDHERIGEL_API AACMQCollider : public AActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	AACMQCollider();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

public:	

	UPROPERTY()
	class AActor* ColliderOwner;

	UPROPERTY()
		bool IsUpgraded;

	UPROPERTY(VisibleAnywhere)
	class UStaticMeshComponent* MeshComponent;

protected:

	UPROPERTY()
	float CurrentDamage;

	UPROPERTY()
	float Damage = 10.f;

	UPROPERTY()
	float UpgradedDamage = 15.f;

	UPROPERTY()
	float CurrentRange;

	UPROPERTY()
	float Range = 1.f;

	UPROPERTY()
	float UpgradedRange = 2.f;

	UPROPERTY()
	float DecreasingDefensePointAmount;

	UPROPERTY()
	float DecreasingDefensePointTime;

protected:
	UFUNCTION()
		void OnColliderOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult);

public:
	UFUNCTION()
	void InitStatus();

};
