// Fill out your copyright notice in the Description page of Project Settings.


#include "ColdEProjectile.h"
#include "GameFramework/ProjectileMovementComponent.h"
#include "Components/SphereComponent.h"

// Sets default values
AColdEProjectile::AColdEProjectile()
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
	if (!ProjectileMesh)
	{
		ProjectileMesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("ProjectileMesh"));
		ProjectileMesh->SetupAttachment(CollisionComponent);

		static ConstructorHelpers::FObjectFinder<UStaticMesh>Mesh(TEXT("/Game/Heros/Cold/Skill/SM_ColdWProjectile"));
		if (Mesh.Succeeded())
		{
			ProjectileMesh->SetStaticMesh(Mesh.Object);
		}

	}
	if (!ProjectileMovementComponent)
	{
		// Use this component to drive this projectile's movement.
		ProjectileMovementComponent = CreateDefaultSubobject<UProjectileMovementComponent>(TEXT("ProjectileMovementComponent"));
		ProjectileMovementComponent->SetUpdatedComponent(CollisionComponent);
		ProjectileMovementComponent->InitialSpeed = 0.f;
		ProjectileMovementComponent->MaxSpeed = 0.f;
		ProjectileMovementComponent->bRotationFollowsVelocity = true;
		ProjectileMovementComponent->bShouldBounce = false;
		ProjectileMovementComponent->Bounciness = 0.f;
		ProjectileMovementComponent->ProjectileGravityScale = 0.0f;
	}
}

// Called when the game starts or when spawned
void AColdEProjectile::BeginPlay()
{
	Super::BeginPlay();
	
}

// Called every frame
void AColdEProjectile::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	if (isReadyToFire)
	{
		
	}
}

void AColdEProjectile::SetLocation(FVector location)
{
	start = projectileOwner->GetActorLocation();

	destination = location;

	direction = destination - start;

	direction.Y = start.Y;

	totalLength = direction.Length();
	

	isReadyToFire = true;
}

