// Fill out your copyright notice in the Description page of Project Settings.

#include "DummyProjectile.h"
#include "GameFramework/ProjectileMovementComponent.h"
#include "Components/SphereComponent.h"
#include "SidheRigel/Interface/Damagable.h"
#include "SidheRigel/Interface/Movable.h"
#include "Math/UnrealMathUtility.h"

// Sets default values
ADummyProjectile::ADummyProjectile()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

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

	static ConstructorHelpers::FObjectFinder<UStaticMesh>Mesh(TEXT("/Game/Dummy/MaterialSphere"));
	if (Mesh.Succeeded())
	{
		ProjectileMesh->SetStaticMesh(Mesh.Object);
	}

	RootComponent = ProjectileMesh;

	CreateDefaultSubobject<USceneComponent>(TEXT("ProjectileSceneComponent"))->SetupAttachment(RootComponent);
}

void ADummyProjectile::SetProjectileMovementComponent()
{
	ProjectileMovementComponent = CreateDefaultSubobject<UProjectileMovementComponent>(TEXT("ProjectileMovementComponent"));
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
		FVector Forward = (Target->GetActorLocation() - GetActorLocation()).GetSafeNormal();
		ProjectileMovementComponent->Velocity = (Forward * speed);
		if ((this->GetDistanceTo(Target)) < 100.f)
		{
			float totalAttackDamage = AttackDamage;

			if (FMath::RandRange(0, 1) <= criticalRate)
			{
				totalAttackDamage *= criticalDamage;
			}

			if (IDamagable* damagableTarget = Cast<IDamagable>(Target))
			{
				damagableTarget->TakeDamage(totalAttackDamage, projectileOwner);
			}

			Target = nullptr;

			Destroy();
		}
	}
	else
	{
		Destroy();
	}
}