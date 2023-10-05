// Fill out your copyright notice in the Description page of Project Settings.


#include "ColdQProjectile.h"
#include "GameFramework/ProjectileMovementComponent.h"
#include "Components/SphereComponent.h"
#include "SidheRigel/Interface/Damagable.h"

// Sets default values
AColdQProjectile::AColdQProjectile()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	if (!ProjectileMesh)
	{
		ProjectileMesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("ProjectileMesh"));

		static ConstructorHelpers::FObjectFinder<UStaticMesh>Mesh(TEXT("/Game/Heros/Cold/Skill/SM_ColdQProjectile"));
		if (Mesh.Succeeded())
		{
			ProjectileMesh->SetStaticMesh(Mesh.Object);
		}

		RootComponent = ProjectileMesh;

		CreateDefaultSubobject<USceneComponent>(TEXT("ProjectileSceneComponent"))->SetupAttachment(RootComponent);
	}
	if (!ProjectileMovementComponent)
	{
		// Use this component to drive this projectile's movement.
		ProjectileMovementComponent = CreateDefaultSubobject<UProjectileMovementComponent>(TEXT("ProjectileMovementComponent"));
		ProjectileMovementComponent->SetUpdatedComponent(ProjectileMesh);
		ProjectileMovementComponent->InitialSpeed = 5.f;
		ProjectileMovementComponent->MaxSpeed = 5000.f;
		ProjectileMovementComponent->bRotationFollowsVelocity = true;
		ProjectileMovementComponent->bShouldBounce = false;
		ProjectileMovementComponent->Bounciness = 0.f;
		ProjectileMovementComponent->ProjectileGravityScale = 0.0f;
	}
}

// Called when the game starts or when spawned
void AColdQProjectile::BeginPlay()
{
	Super::BeginPlay();
	
}

// Called every frame
void AColdQProjectile::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	if (target)
	{
		FVector Forward = (target->GetActorLocation() - GetActorLocation()).GetSafeNormal();
		ProjectileMovementComponent->AddForce(Forward * speed);

		if (GetDistanceTo(target) < 100.f)
		{
			if (IDamagable* damagableTarget = Cast<IDamagable>(target))
			{
				damagableTarget->TakeDamage(damage, projectileOwner);
			}

			Destroy();
		}
	}
	else
	{
		Destroy();
	}
}

