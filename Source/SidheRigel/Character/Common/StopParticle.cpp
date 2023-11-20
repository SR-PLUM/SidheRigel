// Fill out your copyright notice in the Description page of Project Settings.


#include "StopParticle.h"

// Sets default values
AStopParticle::AStopParticle()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	if (!ParticleComponent)
	{
		ParticleComponent = CreateDefaultSubobject <UParticleSystemComponent>(TEXT("Particle"));
	}
}

// Called when the game starts or when spawned
void AStopParticle::BeginPlay()
{
	Super::BeginPlay();
	
}

// Called every frame
void AStopParticle::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	if (target)
	{
		SetActorLocation(target->GetActorLocation() + FVector::DownVector * 100);
	}
}

