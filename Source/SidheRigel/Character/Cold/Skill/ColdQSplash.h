// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "ColdQSplash.generated.h"

UCLASS()
class SIDHERIGEL_API AColdQSplash : public AActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	AColdQSplash();

private:
	UPROPERTY(VisibleAnywhere)
		class USphereComponent* CollisionComponent;

	UPROPERTY()
		float elapsedTime = 0.f;

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	UPROPERTY()
		AActor* projectileOwner;
	
	UFUNCTION()
		void OnOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult);

	float damage = 20.f;
	float particleDuration = 1.f;
};
