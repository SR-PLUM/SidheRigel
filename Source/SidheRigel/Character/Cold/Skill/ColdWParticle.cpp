// Fill out your copyright notice in the Description page of Project Settings.


#include "ColdWParticle.h"

// Sets default values
AColdWParticle::AColdWParticle()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

}

// Called when the game starts or when spawned
void AColdWParticle::BeginPlay()
{
	Super::BeginPlay();
	
	FTimerHandle WParticleTimer;
	GetWorldTimerManager().SetTimer(WParticleTimer,
		FTimerDelegate::CreateLambda([=]()
			{
				Destroy();
			}
	), particleDuration, false);
}

// Called every frame
void AColdWParticle::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

