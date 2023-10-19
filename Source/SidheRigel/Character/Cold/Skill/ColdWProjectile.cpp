// Fill out your copyright notice in the Description page of Project Settings.


#include "ColdWProjectile.h"
#include "GameFramework/ProjectileMovementComponent.h"
#include "Components/SphereComponent.h"
#include "SidheRigel/Interface/Damagable.h"
#include "SidheRigel/Interface/Movable.h"
#include "SidheRigel/Interface/Team.h"

// Sets default values
AColdWProjectile::AColdWProjectile()
{
	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	if (!RootComponent)
	{
		RootComponent = CreateDefaultSubobject<USceneComponent>(TEXT("ProjectileSceneComponent"));
	}
	if (!ProjectileMesh)
	{
		ProjectileMesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("ProjectileMesh"));
		ProjectileMesh->SetupAttachment(RootComponent);

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
		ProjectileMovementComponent->SetUpdatedComponent(RootComponent);
		ProjectileMovementComponent->InitialSpeed = 0.f;
		ProjectileMovementComponent->MaxSpeed = 0.f;
		ProjectileMovementComponent->bRotationFollowsVelocity = true;
		ProjectileMovementComponent->bShouldBounce = false;
		ProjectileMovementComponent->Bounciness = 0.f;
		ProjectileMovementComponent->ProjectileGravityScale = 0.0f;
	}
}

// Called when the game starts or when spawned
void AColdWProjectile::BeginPlay()
{
	Super::BeginPlay();

	ProjectileMesh->OnComponentBeginOverlap.AddDynamic(this, &AColdWProjectile::OnColliderOverlap);

	FTimerHandle WProjectileTimer;
	GetWorldTimerManager().SetTimer(WProjectileTimer, 
		FTimerDelegate::CreateLambda([=]()
			{
				Destroy();
			}
	), duration, false);
}

// Called every frame
void AColdWProjectile::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

void AColdWProjectile::OnColliderOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{
	if (OtherActor)
	{
		if (IDamagable* target = Cast<IDamagable>(OtherActor))
		{
			if (ITeam* TeamActor = Cast<ITeam>(OtherActor))
			{
				if (TeamActor)
				{
					if (TeamActor->GetTeam() != Cast<ITeam>(projectileOwner)->GetTeam())
					{
						target->TakeDamage(damage, projectileOwner);

						if (IMovable* MoveActor = Cast<IMovable>(OtherActor))
						{
							if (projectileOwner)
							{
								FVector moveDirection = (OtherActor->GetActorLocation() - projectileOwner->GetActorLocation());

								MoveActor->MoveVector(moveDirection, force);
							}
						}
					}
				}
			}
		}
	}
}

