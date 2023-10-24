// Fill out your copyright notice in the Description page of Project Settings.


#include "Tower.h"

#include "Components/SphereComponent.h"
#include "Components/StaticMeshComponent.h"
#include "Components/WidgetComponent.h"

#include "SidheRigel/UI/HPUI.h"
#include "SidheRigel/Tower/TowerAttackProjectile.h"

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
		rangeArea->InitSphereRadius(500.0f);
		rangeArea->SetupAttachment(mesh);
	}

	static ConstructorHelpers::FObjectFinder<UBlueprint> projectileRef(TEXT("/Game/Tower/BP_TowerAttackProjectile"));
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
						projectile->Target = currentTarget;
						projectile->AttackDamage = damage;
						
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

				attackList.Add(OtherActor);
				UE_LOG(LogTemp, Warning, TEXT("ENTER ENEMY IN TOWER : %s"), *OtherActor->GetName());
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
				}
			}
			UE_LOG(LogTemp, Warning, TEXT("EXIT ENEMY IN TOWER : %s"), *OtherActor->GetName());
		}
	}
}

void ATower::TakeDamage(float _damage, AActor* damageCauser)
{
	HP -= _damage;

	TowerUIRef->SetHPBar(HP/ MaxHP);

	if (HP <= 0)
	{
		//Destroy Animation
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

