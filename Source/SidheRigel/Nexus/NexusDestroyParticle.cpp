// Fill out your copyright notice in the Description page of Project Settings.


#include "NexusDestroyParticle.h"

// Sets default values
ANexusDestroyParticle::ANexusDestroyParticle()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

}

// Called when the game starts or when spawned
void ANexusDestroyParticle::BeginPlay()
{
	Super::BeginPlay();

	FTimerHandle DestroyParticleTimer;
	GetWorldTimerManager().SetTimer(DestroyParticleTimer,
		FTimerDelegate::CreateLambda([=]()
			{
				Destroy();
			}
	), particleDuration, false);
}

// Called every frame
void ANexusDestroyParticle::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

