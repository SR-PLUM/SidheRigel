// Fill out your copyright notice in the Description page of Project Settings.


#include "MinionProjectile.h"
#include "GameFramework/ProjectileMovementComponent.h"

#include "../Interface/Damagable.h"

// Sets default values
AMinionProjectile::AMinionProjectile()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	if (!ProjectileMesh)
	{
		ProjectileMesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("ProjectileMesh"));

		static ConstructorHelpers::FObjectFinder<UStaticMesh>Mesh(TEXT("/Game/Dummy/MaterialSphere"));
		if (Mesh.Succeeded())
		{
			ProjectileMesh->SetStaticMesh(Mesh.Object);
		}

		RootComponent = ProjectileMesh;
	}

	if (!ProjectileMovementComponent)
	{
		ProjectileMovementComponent = CreateDefaultSubobject<UProjectileMovementComponent>(TEXT("ProjectileMovementComponent"));
		ProjectileMovementComponent->SetUpdatedComponent(ProjectileMesh);
		ProjectileMovementComponent->InitialSpeed = 5.f;
		ProjectileMovementComponent->MaxSpeed = 5.f;
		ProjectileMovementComponent->bRotationFollowsVelocity = true;
		ProjectileMovementComponent->bShouldBounce = false;
		ProjectileMovementComponent->Bounciness = 0.f;
		ProjectileMovementComponent->ProjectileGravityScale = 0.0f;
	}
}

// Called when the game starts or when spawned
void AMinionProjectile::BeginPlay()
{
	Super::BeginPlay();
	
}

// Called every frame
void AMinionProjectile::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	if (target)
	{
		FVector Forward = target->GetActorLocation() - GetActorLocation();
		ProjectileMovementComponent->Velocity = Forward * ProjectileMovementComponent->InitialSpeed;

		if ((this->GetDistanceTo(target)) < 100.f)
		{
			if (IDamagable* damagableTarget = Cast<IDamagable>(target))
			{
				damagableTarget->TakeDamage(attackDamage, projectileOwner);
			}

			Destroy();
		}
	}
}

