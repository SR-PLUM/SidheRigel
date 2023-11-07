// Fill out your copyright notice in the Description page of Project Settings.


#include "FairyWingEProjectile.h"
#include "GameFramework/ProjectileMovementComponent.h"
#include "Components/SphereComponent.h"
#include "SidheRigel/Interface/Damagable.h"
#include "SidheRigel/Interface/Team.h"
#include "SidheRigel/SidheRigelCharacter.h"

// Sets default values
AFairyWingEProjectile::AFairyWingEProjectile()
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

		static ConstructorHelpers::FObjectFinder<UStaticMesh>Mesh(TEXT("/Game/Heros/FairyWing/Skill/SM_FairyWingEProjectile"));
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
		ProjectileMovementComponent->InitialSpeed = 5.f;
		ProjectileMovementComponent->MaxSpeed = 5.f;
		ProjectileMovementComponent->bRotationFollowsVelocity = true;
		ProjectileMovementComponent->bShouldBounce = false;
		ProjectileMovementComponent->Bounciness = 0.f;
		ProjectileMovementComponent->ProjectileGravityScale = 0.0f;
	}

	isHerohit = false;
}

// Called when the game starts or when spawned
void AFairyWingEProjectile::BeginPlay()
{
	Super::BeginPlay();
	
}

// Called every frame
void AFairyWingEProjectile::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	if (target)
	{		
		FVector Forward = target->GetActorLocation() - GetActorLocation();
		ProjectileMovementComponent->Velocity = Forward * ProjectileMovementComponent->InitialSpeed;

		if (ITeam* team = Cast<ITeam>(target))
		{
			if (team->GetTeam() != Cast<ITeam>(projectileOwner)->GetTeam())
			{
				if ((this->GetDistanceTo(target)) < 100.f)
				{
					if (IDamagable* enemy = Cast<IDamagable>(target))
					{
						enemy->TakeDamage(damage, projectileOwner);

						if (target->ActorHasTag("Hero"))
							isHerohit = true;
					}
						
					Destroy();
				}
			}
		}		
	}
}