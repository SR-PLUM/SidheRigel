// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "ColdEDamageField.generated.h"

UCLASS()
class SIDHERIGEL_API AColdEDamageField : public AActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	AColdEDamageField();

private:
	UPROPERTY(VisibleAnywhere)
		class USphereComponent* CollisionComponent;

	UPROPERTY(VisibleAnywhere)
		class UStaticMeshComponent* ProjectileMesh;

	virtual void NotifyActorBeginOverlap(AActor* OtherActor) override;
	virtual void NotifyActorEndOverlap(AActor* OtherActor) override;

	TArray<IDamagable*> targets;

	TSubclassOf<class AColdEGroundFireEffect> groundFireClass;
protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	UPROPERTY()
		AActor* projectileOwner;

	float fieldArea;
	float damage;

	float fieldDamage;

	void Explosion();

	bool canEnter = true;

	float talentKillThreshold = 100.f;
};
