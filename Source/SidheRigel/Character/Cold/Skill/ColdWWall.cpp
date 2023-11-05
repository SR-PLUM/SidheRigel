// Fill out your copyright notice in the Description page of Project Settings.


#include "ColdWWall.h"

// Sets default values
AColdWWall::AColdWWall()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

}

// Called when the game starts or when spawned
void AColdWWall::BeginPlay()
{
	Super::BeginPlay();

	FTimerHandle WProjectileTimer;
	GetWorldTimerManager().SetTimer(WProjectileTimer,
		FTimerDelegate::CreateLambda([=]()
			{
				Destroy();
			}
	), duration, false);
	
}

// Called every frame
void AColdWWall::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

