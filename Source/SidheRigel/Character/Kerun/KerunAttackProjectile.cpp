// Fill out your copyright notice in the Description page of Project Settings.


#include "KerunAttackProjectile.h"
#include "GameFramework/ProjectileMovementComponent.h"
#include "Components/SphereComponent.h"
#include "../../Interface/Damagable.h"
#include "Math/UnrealMathUtility.h"

// Sets default values
AKerunAttackProjectile::AKerunAttackProjectile()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

}

// Called when the game starts or when spawned
void AKerunAttackProjectile::BeginPlay()
{
	Super::BeginPlay();
	
}

// Called every frame
void AKerunAttackProjectile::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

void AKerunAttackProjectile::SetProjectileMesh()
{
}

void AKerunAttackProjectile::SetProjectileMovementComponent()
{
	ProjectileMovementComponent = CreateDefaultSubobject<UProjectileMovementComponent>(TEXT("ProjectileMovementComponent"));
	ProjectileMovementComponent->InitialSpeed = 5.f;
	ProjectileMovementComponent->MaxSpeed = 5.f;
	ProjectileMovementComponent->bRotationFollowsVelocity = true;
	ProjectileMovementComponent->bShouldBounce = false;
	ProjectileMovementComponent->Bounciness = 0.f;
	ProjectileMovementComponent->ProjectileGravityScale = 0.0f;
}

