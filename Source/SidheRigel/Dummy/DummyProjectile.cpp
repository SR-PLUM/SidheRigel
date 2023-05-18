// Fill out your copyright notice in the Description page of Project Settings.

#include "DummyProjectile.h"
#include "GameFramework/ProjectileMovementComponent.h"
#include "Components/SphereComponent.h"
#include "SidheRigel/Interface/Damagable.h"
#include "Math/UnrealMathUtility.h"

// Sets default values
ADummyProjectile::ADummyProjectile()
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
		SetProjectileMesh();
	}
	if (!ProjectileMovementComponent)
	{
		SetProjectileMovementComponent();
	}
}

// Called when the game starts or when spawned
void ADummyProjectile::BeginPlay()
{
	Super::BeginPlay();
	
}

void ADummyProjectile::SetProjectileMesh()
{
	ProjectileMesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("ProjectileMesh"));
	ProjectileMesh->SetupAttachment(CollisionComponent);

	static ConstructorHelpers::FObjectFinder<UStaticMesh>Mesh(TEXT("/Game/Dummy/MaterialSphere"));
	if (Mesh.Succeeded())
	{
		ProjectileMesh->SetStaticMesh(Mesh.Object);
	}
}

void ADummyProjectile::SetProjectileMovementComponent()
{
	ProjectileMovementComponent = CreateDefaultSubobject<UProjectileMovementComponent>(TEXT("ProjectileMovementComponent"));
	ProjectileMovementComponent->SetUpdatedComponent(CollisionComponent);
	ProjectileMovementComponent->InitialSpeed = 5.f;
	ProjectileMovementComponent->MaxSpeed = 5.f;
	ProjectileMovementComponent->bRotationFollowsVelocity = true;
	ProjectileMovementComponent->bShouldBounce = false;
	ProjectileMovementComponent->Bounciness = 0.f;
	ProjectileMovementComponent->ProjectileGravityScale = 0.0f;
}

// Called every frame
void ADummyProjectile::Tick(float DeltaTime)
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

			Cast<IDamagable>(Target)->TakeDamage(totalAttackDamage, projectileOwner);
			Destroy();
		}
	}
}