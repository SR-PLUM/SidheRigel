// Fill out your copyright notice in the Description page of Project Settings.


#include "ColdEDamageField.h"

#include "GameFramework/ProjectileMovementComponent.h"
#include "Components/SphereComponent.h"

#include "SidheRigel/Interface/Damagable.h"

// Sets default values
AColdEDamageField::AColdEDamageField()
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
		CollisionComponent->InitSphereRadius(fieldArea);
		// Set the root component to be the collision component.
		RootComponent = CollisionComponent;
	}
	if (!ProjectileMesh)
	{
		ProjectileMesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("ProjectileMesh"));
		ProjectileMesh->SetupAttachment(CollisionComponent);

		static ConstructorHelpers::FObjectFinder<UStaticMesh>Mesh(TEXT("/Game/Heros/Cold/Skill/SM_ColdEDamageField"));
		if (Mesh.Succeeded())
		{
			ProjectileMesh->SetStaticMesh(Mesh.Object);
		}

	}
}

void AColdEDamageField::NotifyActorBeginOverlap(AActor* OtherActor)
{
	IDamagable* target = Cast<IDamagable>(OtherActor);
	if (target)
	{
		targets.Add(target);
	}
}

void AColdEDamageField::NotifyActorEndOverlap(AActor* OtherActor)
{
	IDamagable* target = Cast<IDamagable>(OtherActor);
	if (target)
	{
		targets.Remove(target);
	}
}

// Called when the game starts or when spawned
void AColdEDamageField::BeginPlay()
{
	Super::BeginPlay();
}

// Called every frame
void AColdEDamageField::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

void AColdEDamageField::Explosion()
{
	for (auto target : targets)
	{
		target->TakeDamage(damage, projectileOwner);
	}

	Destroy();
}

