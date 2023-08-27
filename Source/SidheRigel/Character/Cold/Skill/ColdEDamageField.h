// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "ColdEDamageField.generated.h"

UCLASS()
class SIDHERIGEL_API AColdEDamageField : public AActor
{
	GENERATED_BODY()
	const float FIELD_AREA = 105.f;
	const float DAMAGE = 10.f;
	
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
protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	UPROPERTY()
		AActor* projectileOwner;

	void Explosion();
};
