// Fill out your copyright notice in the Description page of Project Settings.

#include "ColdEProjectile.h"

#include "GameFramework/ProjectileMovementComponent.h"
#include "Kismet/GameplayStatics.h"

#include "ColdEDamageField.h"

// Sets default values
AColdEProjectile::AColdEProjectile()
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

		static ConstructorHelpers::FObjectFinder<UStaticMesh>Mesh(TEXT("/Game/Heros/Cold/Skill/SM_ColdEProjectile"));
		if (Mesh.Succeeded())
		{
			ProjectileMesh->SetStaticMesh(Mesh.Object);
		}

		ProjectileMesh->SetSimulatePhysics(true);

		RootComponent = ProjectileMesh;
	}
	if (!ProjectileMovementComponent)
	{
		// Use this component to drive this projectile's movement.
		ProjectileMovementComponent = CreateDefaultSubobject<UProjectileMovementComponent>(TEXT("ProjectileMovementComponent"));
		ProjectileMovementComponent->SetUpdatedComponent(ProjectileMesh);
		ProjectileMovementComponent->InitialSpeed = 0.f;
		ProjectileMovementComponent->MaxSpeed = 0.f;
		ProjectileMovementComponent->bRotationFollowsVelocity = true;
		ProjectileMovementComponent->bShouldBounce = false;
		ProjectileMovementComponent->Bounciness = 0.f;
		ProjectileMovementComponent->ProjectileGravityScale = 1.0f;
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
	
	if (GetActorLocation().Z <= 0)
	{
		UE_LOG(LogTemp, Warning, TEXT("Location Z is 0"));

		damageField->Explosion();

		Destroy();
	}
}

void AColdEProjectile::Launch(FVector startLoc, FVector targetLoc)
{
	FVector outVelocity = FVector::ZeroVector;   // 결과 Velocity

	UWorld* World = GetWorld();

	if (World != nullptr && UGameplayStatics::SuggestProjectileVelocity_CustomArc(this, outVelocity, startLoc, targetLoc, World->GetGravityZ(), ARC_VALUE))
	{
		ProjectileMesh->AddImpulse(outVelocity);
	}
}

