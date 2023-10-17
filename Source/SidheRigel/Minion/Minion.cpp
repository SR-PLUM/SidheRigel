// Fill out your copyright notice in the Description page of Project Settings.


#include "Minion.h"

#include "MinionAIController.h"
#include "WayPoint.h"
#include "SidheRigel/SidheRigelCharacter.h"
#include "MinionProjectile.h"

#include "Kismet/GameplayStatics.h"
#include "Components/SphereComponent.h"
#include "GameFramework/CharacterMovementComponent.h"

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

	AIControllerClass = AMinionAIController::StaticClass();
	AutoPossessAI = EAutoPossessAI::PlacedInWorldOrSpawned;

	static ConstructorHelpers::FObjectFinder<UBlueprint> projectileRef(TEXT("/Game/Minion/BP_MinionProjectile"));
	if (projectileRef.Object)
	{
		projectileClass = (UClass*)projectileRef.Object->GeneratedClass;
	}

	InitMinionWidget();
}

// Called when the game starts or when spawned
void AMinion::BeginPlay()
{
	Super::BeginPlay();
	
	UGameplayStatics::GetAllActorsOfClass(GetWorld(), AWayPoint::StaticClass(), WayPoints);

	detectArea->OnComponentBeginOverlap.AddDynamic(this, &AMinion::OnEnterEnemy);
	detectArea->OnComponentEndOverlap.AddDynamic(this, &AMinion::OnExitEnemy);

	AIController = Cast<AMinionAIController>(GetController());

	if (team == E_Team::Red)
	{
		currentWayPointOrder = 0;
	}
	else
	{
		currentWayPointOrder = WayPoints.Num() - 1;
	}

	MoveToWayPoint();

	InitMinionUI();
}

// Called every frame
void AMinion::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	//If Goal WayPoint Move To Next WayPoint
	if (currentWayPoint)
	{
		if (WayPoints.Num() > 0)
		{
			if (GetDistanceTo(currentWayPoint) <= 100.f)
			{
				if (team == E_Team::Red)
				{
					currentWayPointOrder++;
				}
				else
				{
					currentWayPointOrder--;
				}
				WayPoints.Remove(currentWayPoint);
				MoveToWayPoint();
			}
		}
	}
	else
	{
		MoveToWayPoint();
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
		IDamagable* damagableTarget = Cast<IDamagable>(currentTarget);
		if (damagableTarget)
		{
			if (damagableTarget->GetHP() <= 0)
			{
				attackList.Remove(currentTarget);

				if (attackList.Num() == 0)
				{
					currentTarget = nullptr;
					MoveToWayPoint();
				}
				else
				{
					currentTarget = attackList.Top();
				}
			}
			else if (GetDistanceTo(currentTarget) <= range && attackDelay <= 0)
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
					AIController->MoveToActor(currentTarget, range - 80);
				}
				else
				{
					AIController = Cast<AMinionAIController>(GetController());
					UE_LOG(LogTemp, Warning, TEXT("MINION GET_CONTROLLER"))
				}
			}
		}

		
	}
}

// Called to bind functionality to input
void AMinion::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);

}

void AMinion::MoveToWayPoint()
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
				UE_LOG(LogTemp, Warning, TEXT("ENTER ENEMY : %s"), *OtherActor->GetName());
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
			UE_LOG(LogTemp, Warning, TEXT("EXIT ENEMY : %s"), *OtherActor->GetName());
		}
	}
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
		MinionUIRef->SetUIVisibility(false);
	}
}

void AMinion::Attack(AActor* Target)
{
}

void AMinion::Stun(float time)
{
}

void AMinion::Stop(float time)
{
}

void AMinion::Slow(float time, float value, FString key)
{
}

void AMinion::Silence(float time)
{
}

void AMinion::Airborne(float time)
{
}

void AMinion::TakeDamage(float _damage, AActor* damageCauser)
{
	hp -= _damage;
	UE_LOG(LogTemp, Warning, TEXT("Minion_HP : %f"), hp);

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
}
