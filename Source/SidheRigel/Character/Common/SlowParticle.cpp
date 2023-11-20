// Fill out your copyright notice in the Description page of Project Settings.


#include "SlowParticle.h"

// Sets default values
ASlowParticle::ASlowParticle()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	if (!ParticleComponent)
	{
		ParticleComponent = CreateDefaultSubobject <UParticleSystemComponent>(TEXT("Particle"));
	}
}

// Called when the game starts or when spawned
void ASlowParticle::BeginPlay()
{
	Super::BeginPlay();
	
}

// Called every frame
void ASlowParticle::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	if (target)
	{
		SetActorLocation(target->GetActorLocation() + FVector::DownVector * 100);
	}
}

