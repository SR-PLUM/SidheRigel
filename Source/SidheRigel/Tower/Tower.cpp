// Fill out your copyright notice in the Description page of Project Settings.


#include "Tower.h"

#include "Components/SphereComponent.h"
#include "Components/StaticMeshComponent.h"
#include "Components/WidgetComponent.h"
#include "Particles/ParticleSystemComponent.h"
#include "kismet/GameplayStatics.h"

#include "SidheRigel/UI/HPUI.h"
#include "SidheRigel/Tower/TowerAttackProjectile.h"
#include "TowerDestroyParticle.h"
#include "SidheRigel/SidheRigelGameInstance.h"
#include "SidheRigel/SidheRigelCharacter.h"
#include "SidheRigel/Nexus/Nexus.h"

// Sets default values
ATower::ATower()
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

		static ConstructorHelpers::FObjectFinder<UStaticMesh>meshRef(TEXT("/Game/Tower/S_Modular_Gate_Pillars_xfxoeigiy_lod0_Var1"));
		if (meshRef.Succeeded())
		{
			mesh->SetStaticMesh(meshRef.Object);
		}
		RootComponent = mesh;
	}

	if (!rangeArea)
	{
		rangeArea = CreateDefaultSubobject<USphereComponent>(TEXT("rangeArea"));
		rangeArea->InitSphereRadius(range);
		rangeArea->SetupAttachment(mesh);
	}

	if (!destroyParticle)
	{
		destroyParticle = CreateAbstractDefaultSubobject<UParticleSystemComponent>(TEXT("destroyParticle"));
		destroyParticle->SetupAttachment(mesh);
		destroyParticle->SetAutoActivate(false);
	}

	static ConstructorHelpers::FObjectFinder<UClass> projectileRef(TEXT("Blueprint'/Game/Tower/BP_TowerAttackProjectile.BP_TowerAttackProjectile_C'"));
	if (projectileRef.Object)
	{
		projectileClass = (UClass*)projectileRef.Object;
	}

	static ConstructorHelpers::FObjectFinder<UClass>particleRef(TEXT("Blueprint'/Game/Tower/BP_DestroyTowerParticle.BP_DestroyTowerParticle_C'"));
	if (particleRef.Object)
	{
		particleClass = (UClass*)particleRef.Object;
	}

	if (!muzzleLocation)
	{
		muzzleLocation = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("MuzzleLocation"));
		muzzleLocation->SetupAttachment(mesh);
	}

	HP = MaxHP;
	InitTowerWidget();
}

// Called when the game starts or when spawned
void ATower::BeginPlay()
{
	Super::BeginPlay();
	
	rangeArea->OnComponentBeginOverlap.AddDynamic(this, &ATower::OnEnterEnemy);
	rangeArea->OnComponentEndOverlap.AddDynamic(this, &ATower::OnExitEnemy);

	InitTowerUI();
}

void ATower::InitTowerWidget()
{
	TowerWidget = CreateDefaultSubobject<UWidgetComponent>(TEXT("TowerWIDGET"));
	TowerWidget->SetupAttachment(mesh);

	TowerWidget->SetRelativeLocation(FVector(0, 0, 240));
	TowerWidget->SetWidgetSpace(EWidgetSpace::Screen);
	static ConstructorHelpers::FClassFinder<UUserWidget> StatUI(TEXT("/Game/UIBlueprints/InGameUI/WBP_HPUI"));
	if (StatUI.Succeeded())
	{
		TowerWidget->SetWidgetClass(StatUI.Class);
		TowerWidget->SetDrawSize(FVector2D(180, 30));
	}
}

void ATower::InitTowerUI()
{
	auto TmpWidget = Cast<UHPUI>(TowerWidget->GetUserWidgetObject());
	if (nullptr != TmpWidget)
	{
		TowerUIRef = TmpWidget;
		TowerUIRef->InitHPBar();

		auto gameInstance = Cast<USidheRigelGameInstance>(GetGameInstance());
		if (GetTeam() != gameInstance->myTeam)
		{
			TowerUIRef->SetEnemyColor();
		}
	}
}

// Called every frame
void ATower::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	if (attackDelay > 0)
	{
		attackDelay -= DeltaTime;
	}

	if (currentTarget)
	{
		if (GetDistanceTo(currentTarget) > range)
		{
			auto closeActor = GetCloseEnemy();
			if (closeActor)
			{
				if (GetDistanceTo(closeActor) > range)
				{
					return;
				}
				else
				{
					currentTarget = closeActor;
				}
			}
			else
			{
				return;
			}
		}

		DrawDebugLine(GetWorld(), this->GetActorLocation() + FVector(0, 0, 250), currentTarget->GetActorLocation(), FColor(255, 0, 0));

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

					ATowerAttackProjectile* projectile = World->SpawnActorDeferred<ATowerAttackProjectile>(projectileClass, SpawnTransform);
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

void ATower::OnEnterEnemy(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
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

				UE_LOG(LogTemp, Warning, TEXT("%s :: %f"), *OtherActor->GetName(), GetDistanceTo(OtherActor));
				UE_LOG(LogTemp, Warning, TEXT("%s"), *OtherComp->GetName())

				attackList.Add(OtherActor);
			}
		}
	}
}

void ATower::OnExitEnemy(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex)
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
					if (auto characterTarget = Cast<ASidheRigelCharacter>(currentTarget))
					{
						if (attackList.Num() != 1)
						{
							attackList.Remove(currentTarget);
							if(!attackList.IsEmpty())
								currentTarget = attackList.Top();
							attackList.Push(characterTarget);
						}
					}
				}
			}
		}
	}
}

void ATower::TakeDamage(float _damage, AActor* damageCauser)
{
	if (HP <= 0)	return;

	HP -= _damage;

	TowerUIRef->SetHPBar(HP/ MaxHP);

	if (HP <= 0)
	{
		FActorSpawnParameters SpawnParams;
		FTransform SpawnTransform;
		SpawnTransform.SetLocation(GetActorLocation());
		SpawnTransform.SetRotation(GetActorRotation().Quaternion());
		SpawnParams.Owner = this;
		SpawnParams.SpawnCollisionHandlingOverride = ESpawnActorCollisionHandlingMethod::AlwaysSpawn;

		nexus->bIsTowerDestroyed = true;

		//Destroy Animation
		UGameplayStatics::PlaySoundAtLocation(this, destroySound, this->GetActorLocation());
		ATowerDestroyParticle* particle = GetWorld()->SpawnActorDeferred<ATowerDestroyParticle>(particleClass, SpawnTransform);
		if (particle)
		{
			particle->particleDuration = 1.f;
		}
		particle->FinishSpawning(SpawnTransform);
		Destroy();
	}
}

void ATower::RestoreHP(float value)
{
	
}

float ATower::GetHP()
{
	return HP;
}

E_Team ATower::GetTeam()
{
	return team;
}

AActor* ATower::GetCloseEnemy()
{
	float dist = 30000;
	AActor* retActor = nullptr;
	for (auto actor : attackList)
	{
		auto tempDist = GetDistanceTo(actor);
		if (tempDist < dist)
		{
			dist = tempDist;
			retActor = actor;
		}
	}

	return retActor;
}

