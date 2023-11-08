// Fill out your copyright notice in the Description page of Project Settings.


#include "ColdR2Laser.h"

// Sets default values
AColdR2Laser::AColdR2Laser()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

}

// Called when the game starts or when spawned
void AColdR2Laser::BeginPlay()
{
	Super::BeginPlay();

	FTimerHandle R2LaserTimer;
	GetWorldTimerManager().SetTimer(R2LaserTimer,
		FTimerDelegate::CreateLambda([=]()
			{
				Destroy();
			}
	), duration, false);
}

// Called every frame
void AColdR2Laser::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

