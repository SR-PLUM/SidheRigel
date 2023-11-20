// Fill out your copyright notice in the Description page of Project Settings.


#include "Nexus.h"

#include "Components/SphereComponent.h"
#include "Components/StaticMeshComponent.h"
#include "Components/WidgetComponent.h"
#include "Particles/ParticleSystemComponent.h"
#include "kismet/GameplayStatics.h"

#include "SidheRigel/UI/HPUI.h"
#include "SidheRigel/Nexus/NexusAttackProjectile.h"

// Sets default values
ANexus::ANexus()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	if (!RootComponent)
	{
		RootComponent = CreateDefaultSubobject<USceneComponent>(TEXT("SceneComponent"));
	}

	if (!mesh)
	{
		mesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("Mesh"));
		mesh->SetupAttachment(rangeArea);

		static ConstructorHelpers::FObjectFinder<UStaticMesh>meshRef(TEXT("/Game/Nexus/SM_Nexus"));
		if (meshRef.Succeeded())
		{
			mesh->SetStaticMesh(meshRef.Object);
		}
		RootComponent = mesh;
	}

	if (!rangeArea)
	{
		rangeArea = CreateDefaultSubobject<USphereComponent>(TEXT("rangeArea"));
		rangeArea->InitSphereRadius(30.0f);
		rangeArea->SetupAttachment(mesh);
	}

	if (!destroyParticle)
	{
		destroyParticle = CreateAbstractDefaultSubobject<UParticleSystemComponent>(TEXT("destroyParticle"));
		destroyParticle->SetupAttachment(mesh);
		destroyParticle->SetAutoActivate(false);
	}

	static ConstructorHelpers::FObjectFinder<UBlueprint> projectileRef(TEXT("/Game/Nexus/BP_NexusAttackProjectile"));
	if (projectileRef.Object)
	{
		projectileClass = (UClass*)projectileRef.Object->GeneratedClass;
	}

	if (!muzzleLocation)
	{
		muzzleLocation = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("MuzzleLocation"));
		muzzleLocation->SetupAttachment(mesh);
	}

	HP = MaxHP;
}

void ANexus::BeginPlay()
{
	Super::BeginPlay();

	rangeArea->OnComponentBeginOverlap.AddDynamic(this, &ANexus::OnEnterEnemy);
	rangeArea->OnComponentEndOverlap.AddDynamic(this, &ANexus::OnExitEnemy);
}

// Called when the game starts or when spawned


// Called every frame
void ANexus::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	if (attackDelay > 0)
	{
		attackDelay -= DeltaTime;
	}

	if (currentTarget)
	{
		DrawDebugLine(GetWorld(), this->GetActorLocation() + FVector(0, 0, 150), currentTarget->GetActorLocation(), FColor(255, 0, 0));	

		IDamagable* damagableTarget = Cast<IDamagable>(currentTarget);
		if (damagableTarget)
		{
			if (damagableTarget->GetHP() <= 0)
			{
				attackList.Remove(currentTarget);

				if (attackList.Num() == 0)
				{
					currentTarget = nullptr;
				}
				else
				{
					currentTarget = attackList.Top();
				}
			}
			else if (attackDelay <= 0)
			{
				FVector MuzzleLocation = muzzleLocation->GetComponentLocation();
				FRotator MuzzleRotation = GetActorRotation();

				UWorld* World = GetWorld();
				if (World)
				{
					FActorSpawnParameters SpawnParams;
					FTransform SpawnTransform;
					SpawnTransform.SetLocation(MuzzleLocation);
					SpawnTransform.SetRotation(MuzzleRotation.Quaternion());
					SpawnParams.Owner = this;
					SpawnParams.SpawnCollisionHandlingOverride = ESpawnActorCollisionHandlingMethod::AlwaysSpawn;

					ANexusAttackProjectile* projectile = World->SpawnActorDeferred<ANexusAttackProjectile>(projectileClass, SpawnTransform);
					if (projectile)
					{
						UGameplayStatics::PlaySoundAtLocation(this, AttackSound, this->GetActorLocation());
						projectile->Target = currentTarget;
						projectile->AttackDamage = damage;
						projectile->criticalDamage = 1;
						projectile->criticalRate = 0;

						projectile->projectileOwner = this;
					}
					projectile->FinishSpawning(SpawnTransform);
				}

				attackDelay = maxAttackDelay;
			}
		}
	}
}

void ANexus::OnEnterEnemy(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{
	if (ITeam* Enemy = Cast<ITeam>(OtherActor))
	{
		if (Enemy->GetTeam() != GetTeam())
		{
			if (IDamagable* DamagableActor = Cast<IDamagable>(OtherActor))
			{
				if (attackList.Num() == 0)
				{
					currentTarget = OtherActor;
				}

				attackList.Add(OtherActor);
			}
		}
	}
}

void ANexus::OnExitEnemy(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex)
{
	if (ITeam* Enemy = Cast<ITeam>(OtherActor))
	{
		if (IDamagable* DamagableActor = Cast<IDamagable>(OtherActor))
		{
			attackList.Remove(OtherActor);

			if (currentTarget == OtherActor)
			{
				if (attackList.IsEmpty())
				{
					currentTarget = nullptr;
				}
				else
				{
					currentTarget = attackList.Top();
				}
			}
		}
	}
}

void ANexus::TakeDamage(float _damage, AActor* damageCauser)
{
	HP -= _damage;

	if (HP <= 0)
	{
		//Destroy Animation
		UGameplayStatics::PlaySoundAtLocation(this, destroySound, destroyParticle->GetComponentLocation());
		destroyParticle->Activate();
		Destroy();
	}
}

void ANexus::RestoreHP(float value)
{
}

float ANexus::GetHP()
{
	return HP;
}

E_Team ANexus::GetTeam()
{
	return team;
}

