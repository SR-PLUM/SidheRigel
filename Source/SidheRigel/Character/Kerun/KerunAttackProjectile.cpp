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

	if (!RootComponent)
	{
		RootComponent = CreateDefaultSubobject<USceneComponent>(TEXT("ProjectileSceneComponent"));
	}
	if (!CollisionComponent)
	{
		// Use a sphere as a simple collision representation.
		CollisionComponent = CreateDefaultSubobject<USphereComponent>(TEXT("SphereComponent"));
		// Set the sphere's collision radius.
		CollisionComponent->InitSphereRadius(15.0f);
		// Set the root component to be the collision component.
		RootComponent = CollisionComponent;
	}
	if (!ProjectileMovementComponent)
	{
		// Use this component to drive this projectile's movement.
		ProjectileMovementComponent = CreateDefaultSubobject<UProjectileMovementComponent>(TEXT("ProjectileMovementComponent"));
		ProjectileMovementComponent->SetUpdatedComponent(CollisionComponent);
		ProjectileMovementComponent->InitialSpeed = 5.f;
		ProjectileMovementComponent->MaxSpeed = 5.f;
		ProjectileMovementComponent->bRotationFollowsVelocity = true;
		ProjectileMovementComponent->bShouldBounce = false;
		ProjectileMovementComponent->Bounciness = 0.f;
		ProjectileMovementComponent->ProjectileGravityScale = 0.0f;
	}
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

	if (Target)
	{
		FVector Forward = Target->GetActorLocation() - GetActorLocation();
		ProjectileMovementComponent->Velocity = Forward * ProjectileMovementComponent->InitialSpeed;
		if ((this->GetDistanceTo(Target)) < 100.f)
		{
			float totalAttackDamage = AttackDamage;

			if (FMath::RandRange(0, 1) <= criticalRate)
			{
				UE_LOG(LogTemp, Warning, TEXT("CRITICAL!"));
				totalAttackDamage *= criticalDamage;
			}

			Cast<IDamagable>(Target)->TakeDamage(totalAttackDamage);
			Destroy();
		}
	}

}

