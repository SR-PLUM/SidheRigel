// Fill out your copyright notice in the Description page of Project Settings.


#include "ACMAttackProjectile.h"
#include "GameFramework/ProjectileMovementComponent.h"
#include "Components/SphereComponent.h"

AACMAttackProjectile::AACMAttackProjectile()
{
	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

}

// Called when the game starts or when spawned
void AACMAttackProjectile::BeginPlay()
{
	Super::BeginPlay();

}

// Called every frame
void AACMAttackProjectile::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

void AACMAttackProjectile::SetProjectileMesh()
{
}

void AACMAttackProjectile::SetProjectileMovementComponent()
{
	ProjectileMovementComponent = CreateDefaultSubobject<UProjectileMovementComponent>(TEXT("ProjectileMovementComponent"));
	ProjectileMovementComponent->InitialSpeed = 5.f;
	ProjectileMovementComponent->MaxSpeed = 5.f;
	ProjectileMovementComponent->bRotationFollowsVelocity = true;
	ProjectileMovementComponent->bShouldBounce = false;
	ProjectileMovementComponent->Bounciness = 0.f;
	ProjectileMovementComponent->ProjectileGravityScale = 0.0f;
}