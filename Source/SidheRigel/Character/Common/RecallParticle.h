// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "RecallParticle.generated.h"

UCLASS()
class SIDHERIGEL_API ARecallParticle : public AActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	ARecallParticle();

private:
	UPROPERTY(EditDefaultsOnly)
		UParticleSystemComponent* ParticleComponent;

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	UPROPERTY()
		AActor* target;
};
