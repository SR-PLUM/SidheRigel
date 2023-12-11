// Fill out your copyright notice in the Description page of Project Settings.


#include "Minion.h"

#include "MinionAIController.h"
#include "WayPoint.h"
#include "MinionSpawner.h"
#include "SidheRigel/SidheRigelCharacter.h"
#include "MinionProjectile.h"
#include "SidheRigel/Character/Common/StunParticle.h"
#include "SidheRigel/Character/Common/SlowParticle.h"
#include "SidheRigel/Character/Common/StopParticle.h"
#include "SidheRigel/Character/Common/SilenceParticle.h"
#include "SidheRigel/SidheRigelGameInstance.h"

#include "Kismet/GameplayStatics.h"
#include "Components/SphereComponent.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "Net/UnrealNetwork.h"

#include "Components/WidgetComponent.h"
#include "SidheRigel/UI/HPUI.h"

// Sets default values
AMinion::AMinion()
{
 	// Set this character to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	if (!detectArea)
	{
		detectArea = CreateDefaultSubobject<USphereComponent>(TEXT("detectArea"));
		detectArea->InitSphereRadius(500.0f);
		detectArea->SetupAttachment(RootComponent);
	}

	GetCharacterMovement()->MaxWalkSpeed = 325.f;
	currentSpeed = GetCharacterMovement()->MaxWalkSpeed;

	AIControllerClass = AMinionAIController::StaticClass();
	AutoPossessAI = EAutoPossessAI::PlacedInWorldOrSpawned;

	static ConstructorHelpers::FObjectFinder<UBlueprint> projectileRef(TEXT("/Game/Minion/BP_MinionProjectile"));
	if (projectileRef.Object)
	{
		projectileClass = (UClass*)projectileRef.Object->GeneratedClass;
	}

	//StunParticle
	static ConstructorHelpers::FObjectFinder<UBlueprint> StunParticle(TEXT("/Game/Heros/Common/BP_StunParticle"));
	if (StunParticle.Object)
	{
		stunParticleClass = (UClass*)StunParticle.Object->GeneratedClass;
	}
	//SlowParticle
	static ConstructorHelpers::FObjectFinder<UBlueprint> SlowParticle(TEXT("/Game/Heros/Common/BP_SlowParticle"));
	if (SlowParticle.Object)
	{
		slowParticleClass = (UClass*)SlowParticle.Object->GeneratedClass;
	}
	//StopParticle
	static ConstructorHelpers::FObjectFinder<UBlueprint> StopParticle(TEXT("/Game/Heros/Common/BP_StopParticle"));
	if (StopParticle.Object)
	{
		stopParticleClass = (UClass*)StopParticle.Object->GeneratedClass;
	}
	//SilenceParticle
	static ConstructorHelpers::FObjectFinder<UBlueprint> SlienceParticle(TEXT("/Game/Heros/Common/BP_SilenceParticle"));
	if (SlienceParticle.Object)
	{
		silenceParticleClass = (UClass*)SlienceParticle.Object->GeneratedClass;
	}

	static ConstructorHelpers::FObjectFinder<USkeletalMesh> WMMesh(TEXT("/Game/Minion/Minion_Lane_Melee_Dawn"));
	if (WMMesh.Succeeded())
	{
		whiteMeleeMinionMesh = WMMesh.Object;
	}
	static ConstructorHelpers::FObjectFinder<USkeletalMesh> BMMesh(TEXT("/Game/Minion/Minion_Lane_Melee_Dusk"));
	if (BMMesh.Succeeded())
	{
		blackMeleeMinionMesh = BMMesh.Object;
	}

	InitMinionWidget();

	bReplicates = true;
}

void AMinion::GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const
{
	Super::GetLifetimeReplicatedProps(OutLifetimeProps);

	DOREPLIFETIME(AMinion, team);
	DOREPLIFETIME(AMinion, ownerSpawner);
}

// Called when the game starts or when spawned
void AMinion::BeginPlay()
{
	Super::BeginPlay();

	InitProperty();
	
	UGameplayStatics::GetAllActorsOfClass(GetWorld(), AWayPoint::StaticClass(), WayPoints);

	detectArea->OnComponentBeginOverlap.AddDynamic(this, &AMinion::OnEnterEnemy);
	detectArea->OnComponentEndOverlap.AddDynamic(this, &AMinion::OnExitEnemy);

	AIController = Cast<AMinionAIController>(GetController());

	if (team == E_Team::Blue)
	{
		currentWayPointOrder = 1;
	}
	else
	{
		currentWayPointOrder = WayPoints.Num() - 2;
	}

	if (HasAuthority())
	{
		MoveToWayPoint();
		if (GetTeam() == E_Team::Blue)
		{
			currentWayPoint->currentBlueMinion++;
		}
		else
		{
			currentWayPoint->currentRedMinion++;
		}
	}

	InitMinionUI();

	if (GetTeam() == E_Team::Blue)
	{
		GetMesh()->SetSkeletalMesh(whiteMeleeMinionMesh);
	}
	else
	{
		GetMesh()->SetSkeletalMesh(blackMeleeMinionMesh);
	}

	SetOwner(ownerSpawner);
}

// Called every frame
void AMinion::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	if (HasAuthority())
	{
		//If Goal WayPoint Move To Next WayPoint
		if (currentWayPoint)
		{
			if (WayPoints.Num() > 0)
			{
				if (GetDistanceTo(currentWayPoint) <= 100.f)
				{
					if (team == E_Team::Blue)
					{
						currentWayPointOrder++;
						currentWayPoint->currentBlueMinion--;
					}
					else
					{
						currentWayPointOrder--;
						currentWayPoint->currentRedMinion--;
					}
					WayPoints.Remove(currentWayPoint);
					MoveToWayPoint();
					if (team == E_Team::Blue)
					{
						currentWayPoint->currentBlueMinion++;
					}
					else
					{
						currentWayPoint->currentRedMinion++;
					}
				}
			}
		}
		else
		{
			MoveToWayPoint();
		}
	}

	if (attackDelay > 0)
	{
		attackDelay -= DeltaTime;
		if (attackDelay <= 0)
		{
			IsAttackAnim = false;
		}
	}

	if (currentTarget)
	{
		Attack(nullptr);
	}
}

// Called to bind functionality to input
void AMinion::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);

}

void AMinion::MoveToWayPoint_Implementation()
{
	if (HasAuthority())
	{
		if (AIController)
		{
			for (auto wayPoint : WayPoints)
			{
				AWayPoint* wayPointItr = Cast<AWayPoint>(wayPoint);

				if (wayPointItr)
				{
					if (wayPointItr->wayPointOrder == currentWayPointOrder)
					{
						currentWayPoint = wayPointItr;
						AIController->MoveToActor(wayPointItr);
					}
				}
			}
		}
		else
		{

		}
	}
}

void AMinion::OnEnterEnemy(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
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

void AMinion::OnExitEnemy(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex)
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

					MoveToWayPoint();
				}
				else
				{
					currentTarget = attackList.Top();
				}
			}
		}
	}
}

void AMinion::InitProperty()
{
	//DEBUG BALNACE
	maxHp = 100;
	hp = maxHp;
	projectileDamage = 5;
	attackDelay = 1.5;
	range = 200;
	gold = 20;
	exp = 60;
}

E_Team AMinion::GetTeam()
{
	return team;
}

void AMinion::SetTeam(E_Team _team)
{
	team = _team;
}

void AMinion::InitMinionWidget()
{
	MinionWidget = CreateDefaultSubobject<UWidgetComponent>(TEXT("MinionWIDGET"));
	MinionWidget->SetupAttachment(GetMesh());

	MinionWidget->SetRelativeLocation(FVector(0, 0, 240));
	MinionWidget->SetWidgetSpace(EWidgetSpace::Screen);
	static ConstructorHelpers::FClassFinder<UUserWidget> StatUI(TEXT("/Game/UIBlueprints/InGameUI/WBP_HPUI"));
	if (StatUI.Succeeded())
	{
		MinionWidget->SetWidgetClass(StatUI.Class);
		MinionWidget->SetDrawSize(FVector2D(180, 30));
	}
}

void AMinion::InitMinionUI()
{
	auto TmpWidget = Cast<UHPUI>(MinionWidget->GetUserWidgetObject());
	if (nullptr != TmpWidget)
	{
		MinionUIRef = TmpWidget;
		MinionUIRef->InitHPBar();

		auto gameInstance = Cast<USidheRigelGameInstance>(GetGameInstance());
		if (GetTeam() != gameInstance->myTeam)
		{
			MinionUIRef->SetEnemyColor();
		}

		MinionUIRef->SetUIVisibility(false);
	}
}

void AMinion::SpawnStunParticle()
{
	if (stunParticle)
		return;

	UWorld* World = GetWorld();
	if (World)
	{
		FActorSpawnParameters FieldSpawnParams;
		FTransform SpawnTransform;
		SpawnTransform.SetLocation(GetActorLocation() + FVector::UpVector * 100);
		SpawnTransform.SetRotation(GetActorRotation().Quaternion());
		FieldSpawnParams.Owner = this;
		FieldSpawnParams.SpawnCollisionHandlingOverride = ESpawnActorCollisionHandlingMethod::AlwaysSpawn;

		// Spawn the projectile at the muzzle.
		stunParticle = World->SpawnActorDeferred<AStunParticle>(stunParticleClass, SpawnTransform);

		if (stunParticle)
		{
			stunParticle->target = this;
		}

		stunParticle->FinishSpawning(SpawnTransform);
	}
}

void AMinion::RemoveStunParticle()
{
	if (!stunParticle)
		return;

	stunParticle->Destroy();
	stunParticle = nullptr;
}

void AMinion::SpawnSlowParticle()
{
	if (slowParticle)
		return;

	UWorld* World = GetWorld();
	if (World)
	{
		FActorSpawnParameters SpawnParams;
		FTransform SpawnTransform;
		SpawnTransform.SetLocation(GetActorLocation());
		SpawnTransform.SetRotation(GetActorRotation().Quaternion());
		SpawnParams.Owner = this;
		SpawnParams.SpawnCollisionHandlingOverride = ESpawnActorCollisionHandlingMethod::AlwaysSpawn;

		// Spawn the projectile at the muzzle.
		slowParticle = World->SpawnActorDeferred<ASlowParticle>(slowParticleClass, SpawnTransform);

		if (slowParticle)
		{
			slowParticle->target = this;
		}

		slowParticle->FinishSpawning(SpawnTransform);
	}
}

void AMinion::RemoveSlowParticle()
{
	if (!slowParticle)
		return;

	slowParticle->Destroy();
	slowParticle = nullptr;
}

void AMinion::SpawnStopParticle()
{
	if (stopParticle)
		return;

	UWorld* World = GetWorld();
	if (World)
	{
		FTransform SpawnTransform;
		SpawnTransform.SetLocation(GetActorLocation());
		SpawnTransform.SetRotation(GetActorRotation().Quaternion());

		// Spawn the projectile at the muzzle.
		stopParticle = World->SpawnActorDeferred<AStopParticle>(stopParticleClass, SpawnTransform);

		if (stopParticle)
		{
			stopParticle->target = this;
		}

		stopParticle->FinishSpawning(SpawnTransform);
	}
}

void AMinion::RemoveStopParticle()
{
	if (!stopParticle)
		return;

	stopParticle->Destroy();
	stopParticle = nullptr;
}

void AMinion::SpawnSilenceParticle()
{
	if (silenceParticle)
		return;

	UWorld* World = GetWorld();
	if (World)
	{
		FTransform SpawnTransform;
		SpawnTransform.SetLocation(GetActorLocation());
		SpawnTransform.SetRotation(GetActorRotation().Quaternion());

		// Spawn the projectile at the muzzle.
		silenceParticle = World->SpawnActorDeferred<ASilenceParticle>(silenceParticleClass, SpawnTransform);

		if (silenceParticle)
		{
			silenceParticle->target = this;
		}

		silenceParticle->FinishSpawning(SpawnTransform);
	}
}

void AMinion::RemoveSilenceParticle()
{
	if (!silenceParticle)
		return;

	silenceParticle->Destroy();
	silenceParticle = nullptr;
}

void AMinion::Attack(AActor* Target)
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
				if (HasAuthority())
				{
					MoveToWayPoint();
				}
			}
			else
			{
				currentTarget = attackList.Top();
			}
		}
		else if (GetDistanceTo(currentTarget) <= range && attackDelay <= 0 && IsStun == false)
		{
			IsAttackAnim = true;
			FVector MuzzleLocation = GetActorLocation();
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

				AMinionProjectile* projectile = World->SpawnActorDeferred<AMinionProjectile>(projectileClass, SpawnTransform);
				if (projectile)
				{
					projectile->Target = currentTarget;
					projectile->AttackDamage = projectileDamage;
					projectile->criticalDamage = 1;
					projectile->criticalRate = 0;

					projectile->projectileOwner = this;
				}
				projectile->FinishSpawning(SpawnTransform);
			}

			attackDelay = maxAttackDelay;
		}
		else
		{
			if (AIController)
			{
				if (HasAuthority())
				{
					AIController->MoveToActor(currentTarget, range - 80);
				}
			}
			else
			{
				AIController = Cast<AMinionAIController>(GetController());
			}
		}
	}
}

void AMinion::Stun(float time)
{
	IsStun = true;
	GetCharacterMovement()->MaxWalkSpeed = 0;

	SpawnStunParticle();

	GetWorldTimerManager().SetTimer(CheckStunTimer,
		FTimerDelegate::CreateLambda([=]()
			{
				IsStun = false;
				GetCharacterMovement()->MaxWalkSpeed = currentSpeed;

				RemoveStunParticle();
			}

	), time, false);
}

void AMinion::Stop(float time)
{
	GetCharacterMovement()->MaxWalkSpeed = 0;

	SpawnStopParticle();

	GetWorldTimerManager().SetTimer(CheckStunTimer,
		FTimerDelegate::CreateLambda([=]()
			{
				GetCharacterMovement()->MaxWalkSpeed = currentSpeed;

				RemoveStopParticle();
			}

	), time, false);
}

void AMinion::Slow(float time, float value, FString key)
{
	if (speedRate.Contains(key))
	{
		return;
	}

	speedRate.Add(key, value);

	if (time == -1)
		return;

	SpawnSlowParticle();

	FTimerHandle SlowTimer;	//작동 안하지 않나?
	GetWorldTimerManager().SetTimer(SlowTimer, FTimerDelegate::CreateLambda([=]()
		{
			if (speedRate.Find(key))
			{
				speedRate.Remove(key);

				bool hasSlowItem = false;
				for (auto& speedRateItem : speedRate)
				{
					if (speedRateItem.Value < 0)
					{
						hasSlowItem = true;
						break;
					}
				}

				if (!hasSlowItem)
				{
					RemoveSlowParticle();
				}
			}
		})
		, time, false);
}

void AMinion::Silence(float time)
{
	//상태만 게임화면에 표시되게 하기
}

void AMinion::TakeDamage(float _damage, AActor* damageCauser)
{
	hp -= _damage;

	MinionUIRef->SetHPBar(hp / maxHp);
	MinionUIRef->SetUIVisibility(true);

	if (hp <= 0 && !isDie)
	{
		isDie = true;
		auto Character = Cast<ASidheRigelCharacter>(damageCauser);
		if (Character)
		{
			Character->GiveMoney(gold);
		}

		for (auto enemy : attackList)
		{
			if (auto enemyPlayer = Cast<ASidheRigelCharacter>(enemy))
			{
				enemyPlayer->GiveExp(exp);
			}
		}

		Destroy();
	}
}

void AMinion::RestoreHP(float value)
{
	hp += value;
	if (hp > maxHp)
	{
		hp = maxHp;
	}
}

float AMinion::GetHP()
{
	return hp;
}

void AMinion::MoveVector(FVector Direction, float Force)
{
	auto unitDirection = (Direction * FVector(1, 1, 0)).GetSafeNormal();
	LaunchCharacter(unitDirection * Force, false, false);
}
