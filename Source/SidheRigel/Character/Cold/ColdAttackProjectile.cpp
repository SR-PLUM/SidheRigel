// Fill out your copyright notice in the Description page of Project Settings.


#include "ColdAttackProjectile.h"
#include "GameFramework/ProjectileMovementComponent.h"
#include "Components/SphereComponent.h"
#include "SidheRigel/Interface/Damagable.h"
#include "Math/UnrealMathUtility.h"

// Sets default values
AColdAttackProjectile::AColdAttackProjectile()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

}

// Called when the game starts or when spawned
void AColdAttackProjectile::BeginPlay()
{
	Super::BeginPlay();
	
}

// Called every frame
void AColdAttackProjectile::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

void AColdAttackProjectile::SetProjectileMesh()
{
	ProjectileMesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("ProjectileMesh"));
	ProjectileMesh->SetupAttachment(CollisionComponent);

	static ConstructorHelpers::FObjectFinder<UStaticMesh>Mesh(TEXT("/Game/Heros/Cold/SM_ColdAttackProjectile"));
	if (Mesh.Succeeded())
	{
		ProjectileMesh->SetStaticMesh(Mesh.Object);
	}
}

void AColdAttackProjectile::SetProjectileMovementComponent()
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

