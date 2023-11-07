// Fill out your copyright notice in the Description page of Project Settings.


#include "ColdQParticle.h"

// Sets default values
AColdQParticle::AColdQParticle()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

}

// Called when the game starts or when spawned
void AColdQParticle::BeginPlay()
{
	Super::BeginPlay();

	FTimerHandle QParticleTimer;
	GetWorldTimerManager().SetTimer(QParticleTimer,
		FTimerDelegate::CreateLambda([=]()
			{
				Destroy();
			}
	), particleDuration, false);
}

// Called every frame
void AColdQParticle::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

