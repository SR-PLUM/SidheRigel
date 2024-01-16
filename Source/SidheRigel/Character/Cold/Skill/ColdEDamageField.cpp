// Fill out your copyright notice in the Description page of Project Settings.


#include "ColdEDamageField.h"

#include "GameFramework/ProjectileMovementComponent.h"
#include "Components/SphereComponent.h"

#include "SidheRigel/SidheRigelCharacter.h"
#include "SidheRigel/Interface/Damagable.h"
#include "SidheRigel/Interface/Team.h"
#include "SidheRigel/Character/Cold/Skill/ColdEGroundFireEffect.h"

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

	static ConstructorHelpers::FObjectFinder<UClass> groundFireRef(TEXT("Blueprint'/Game/Heros/Cold/Skill/BP_ColdEGroundFireEffect.BP_ColdEGroundFireEffect_C'"));
	if (groundFireRef.Object)
	{
		groundFireClass = (UClass*)groundFireRef.Object;
	}
}

void AColdEDamageField::NotifyActorBeginOverlap(AActor* OtherActor)
{
	if (!canEnter)
		return;

	IDamagable* target = Cast<IDamagable>(OtherActor);
	if (target)
	{
		ITeam* TeamActor = Cast<ITeam>(OtherActor);
		if (TeamActor)
		{
			if (TeamActor->GetTeam() != Cast<ITeam>(projectileOwner)->GetTeam())
			{
				targets.Add(target);
			}
		}
	}
}

void AColdEDamageField::NotifyActorEndOverlap(AActor* OtherActor)
{
	if (!canEnter)
		return;

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

	if (Cast<ASidheRigelCharacter>(projectileOwner)->IsSelectedTalent[2][0])
	{
		SetActorScale3D(FVector(2, 2, 2));
	}
}

// Called every frame
void AColdEDamageField::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

void AColdEDamageField::Explosion()
{
	canEnter = false;

	for (auto target : targets)
	{
		if (target)
		{
			target->TakeDamage(damage, projectileOwner);

			if (Cast<ASidheRigelCharacter>(projectileOwner)->IsSelectedTalent[6][1])
			{
				if (target->GetHP() <= talentKillThreshold)
				{
					//처형 함수
					if (auto characterTarget = Cast<ASidheRigelCharacter>(target))
					{
						characterTarget->Execute();
					}
				}
			}
		}
	}

	UWorld* World = GetWorld();
	if (World)
	{
		FActorSpawnParameters SpawnParams;
		FTransform SpawnTransform;
		SpawnTransform.SetLocation(GetActorLocation());
		SpawnTransform.SetRotation(GetActorRotation().Quaternion());
		SpawnParams.Owner = projectileOwner;
		SpawnParams.SpawnCollisionHandlingOverride = ESpawnActorCollisionHandlingMethod::AlwaysSpawn;

		AColdEGroundFireEffect* groundFire = World->SpawnActorDeferred<AColdEGroundFireEffect>(groundFireClass, SpawnTransform);
		if (groundFire)
		{
			groundFire->damageArea = 100.f;
			groundFire->damage = fieldDamage;
			groundFire->damageCycle = 0.2f;
			groundFire->duration = 1.5f;
			groundFire->projectileOwner = projectileOwner;
		}

		groundFire->FinishSpawning(SpawnTransform);
	}

	Destroy();
}

