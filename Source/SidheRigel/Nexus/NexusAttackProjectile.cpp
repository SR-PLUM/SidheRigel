// Fill out your copyright notice in the Description page of Project Settings.


#include "NexusAttackProjectile.h"
#include "GameFramework/ProjectileMovementComponent.h"

#include "SidheRigel/Interface/Damagable.h"

// Sets default values
ANexusAttackProjectile::ANexusAttackProjectile()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

}

// Called when the game starts or when spawned
void ANexusAttackProjectile::BeginPlay()
{
	Super::BeginPlay();
	
}

// Called every frame
void ANexusAttackProjectile::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

void ANexusAttackProjectile::SetProjectileMesh()
{
	ProjectileMesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("ProjectileMesh"));

	static ConstructorHelpers::FObjectFinder<UStaticMesh>Mesh(TEXT("/Game/Nexus/SM_NexusAttackProjectile"));
	if (Mesh.Succeeded())
	{
		ProjectileMesh->SetStaticMesh(Mesh.Object);
	}

	RootComponent = ProjectileMesh;

	CreateDefaultSubobject<USceneComponent>(TEXT("ProjectileSceneComponent"))->SetupAttachment(RootComponent);
}

void ANexusAttackProjectile::SetProjectileMovementComponent()
{
	ProjectileMovementComponent = CreateDefaultSubobject<UProjectileMovementComponent>(TEXT("ProjectileMovementComponent"));
	ProjectileMovementComponent->SetUpdatedComponent(ProjectileMesh);
	ProjectileMovementComponent->InitialSpeed = 5.f;
	ProjectileMovementComponent->MaxSpeed = 0.f;
	ProjectileMovementComponent->bRotationFollowsVelocity = true;
	ProjectileMovementComponent->bShouldBounce = false;
	ProjectileMovementComponent->Bounciness = 0.f;
	ProjectileMovementComponent->ProjectileGravityScale = 0.0f;
}