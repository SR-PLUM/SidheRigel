// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "KerunR1SkillTalentCollider.generated.h"

UCLASS()
class SIDHERIGEL_API AKerunR1SkillTalentCollider : public AActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	AKerunR1SkillTalentCollider();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

private:
	UPROPERTY(VisibleAnywhere)
		class USphereComponent* CollisionComponent;

	float ColliderDuration = 1.f;
	float SlowAmount = 0.3f;
	float SlowDuration = 2.0f;
	float ColliderForce = 2000.f;

public:
	UPROPERTY()
		AActor* colliderOwner;

	UFUNCTION()
		void OnOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult);

	

};
