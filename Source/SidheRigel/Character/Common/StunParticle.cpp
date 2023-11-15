// Fill out your copyright notice in the Description page of Project Settings.


#include "StunParticle.h"

#include "Particles/ParticleSystemComponent.h"

// Sets default values
AStunParticle::AStunParticle()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	if (!ParticleComponent)
	{
		ParticleComponent = CreateDefaultSubobject <UParticleSystemComponent>(TEXT("Particle"));
	}
}

// Called when the game starts or when spawned
void AStunParticle::BeginPlay()
{
	Super::BeginPlay();
}

// Called every frame
void AStunParticle::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	if (target)
	{
		SetActorLocation(target->GetActorLocation() + FVector::UpVector * 100);
	}
}

