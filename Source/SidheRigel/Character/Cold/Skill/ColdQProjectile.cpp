// Fill out your copyright notice in the Description page of Project Settings.


#include "ColdQProjectile.h"
#include "GameFramework/ProjectileMovementComponent.h"
#include "Components/SphereComponent.h"
#include "kismet/GameplayStatics.h"
#include "SidheRigel/Interface/Damagable.h"

#include "SidheRigel/SidheRigelCharacter.h"
#include "SidheRigel/Character/Cold/Skill/ColdQParticle.h"
#include "SidheRigel/Character/Cold/Skill/ColdQSplash.h"

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

	static ConstructorHelpers::FObjectFinder<UBlueprint>particleRef(TEXT("/Game/Heros/Cold/Skill/BP_ColdQParticle"));
	if (particleRef.Object)
	{
		particleClass = (UClass*)particleRef.Object->GeneratedClass;
	}

	static ConstructorHelpers::FObjectFinder<UBlueprint>splashRef(TEXT("/Game/Heros/Cold/Skill/BP_ColdQSplash"));
	if (splashRef.Object)
	{
		splashClass = (UClass*)splashRef.Object->GeneratedClass;
	}
}

// Called when the game starts or when spawned
void AColdQProjectile::BeginPlay()
{
	Super::BeginPlay();

	UGameplayStatics::PlaySoundAtLocation(this, SpawnSound, this->GetActorLocation());	
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
			if (slowRate != 0)
			{
				if (ICCable* ccTarget = Cast<ICCable>(target))
				{
					ccTarget->Slow(slowTime, slowRate, "Cold_6_2");
				}
			}

			UWorld* World = GetWorld();
			if (World)
			{
				if (auto character = Cast<ASidheRigelCharacter>(projectileOwner))
				{
					if (character->IsSelectedTalent[5][0])
					{
						FActorSpawnParameters SpawnParams;
						FTransform SpawnTransform;
						SpawnTransform.SetLocation(target->GetActorLocation());
						SpawnTransform.SetRotation(GetActorRotation().Quaternion());
						SpawnParams.Owner = projectileOwner;
						SpawnParams.SpawnCollisionHandlingOverride = ESpawnActorCollisionHandlingMethod::AlwaysSpawn;

						AColdQSplash* splash = World->SpawnActorDeferred<AColdQSplash>(splashClass, SpawnTransform);
						if (splash)
						{
							splash->damage = 20.f;
							splash->particleDuration = 1.f;
							splash->projectileOwner = projectileOwner;
						}
						splash->FinishSpawning(SpawnTransform);
					}
					else
					{
						FActorSpawnParameters SpawnParams;
						FTransform SpawnTransform;
						SpawnTransform.SetLocation(GetActorLocation());
						SpawnTransform.SetRotation(GetActorRotation().Quaternion());
						SpawnParams.Owner = projectileOwner;
						SpawnParams.SpawnCollisionHandlingOverride = ESpawnActorCollisionHandlingMethod::AlwaysSpawn;

						AColdQParticle* particle = World->SpawnActorDeferred<AColdQParticle>(particleClass, SpawnTransform);
						if (particle)
						{
							particle->particleDuration = 1.f;
						}
						particle->FinishSpawning(SpawnTransform);
					}
				}
			}

			

			Destroy();
		}
	}
	else
	{
		Destroy();
	}
}

