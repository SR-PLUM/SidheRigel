// Fill out your copyright notice in the Description page of Project Settings.


#include "MinionProjectile.h"
#include "GameFramework/ProjectileMovementComponent.h"

// Sets default values
AMinionProjectile::AMinionProjectile()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;
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
}

void AMinionProjectile::SetProjectileMesh()
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

void AMinionProjectile::SetProjectileMovementComponent()
{
	ProjectileMovementComponent = CreateDefaultSubobject<UProjectileMovementComponent>(TEXT("ProjectileMovementComponent"));
	ProjectileMovementComponent->SetUpdatedComponent(ProjectileMesh);
	ProjectileMovementComponent->InitialSpeed = 10.f;
	ProjectileMovementComponent->MaxSpeed = 0.f;
	ProjectileMovementComponent->bRotationFollowsVelocity = true;
	ProjectileMovementComponent->bShouldBounce = false;
	ProjectileMovementComponent->Bounciness = 0.f;
	ProjectileMovementComponent->ProjectileGravityScale = 0.0f;
}

