// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "ACMECollider.generated.h"

UCLASS()
class SIDHERIGEL_API AACMECollider : public AActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	AACMECollider();

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
	float Damage = 20.f;

	UPROPERTY()
	float UpgradedDamage = 15.f;

	UPROPERTY()
	float StunTime = 1.5f;

	UPROPERTY()
	float AirBoneTime = 1.5f;

protected:
	UFUNCTION()
	void OnColliderOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult);

public:
	UFUNCTION()
	void InitStatus();

};
