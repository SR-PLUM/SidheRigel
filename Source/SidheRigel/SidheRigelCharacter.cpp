// Copyright Epic Games, Inc. All Rights Reserved.

#include "SidheRigelCharacter.h"
#include "Kismet/GameplayStatics.h"
#include "Dummy/DummyProjectile.h"
#include "UObject/ConstructorHelpers.h"
#include "Camera/CameraComponent.h"
#include "Components/DecalComponent.h"
#include "Components/CapsuleComponent.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "GameFramework/PlayerController.h"
#include "GameFramework/SpringArmComponent.h"
#include "Materials/Material.h"
#include "Engine/World.h"
#include "Components/SphereComponent.h"
#include "Components/Overlay.h"
#include "Components/VerticalBox.h"
#include "Components/OverlaySlot.h"
#include "Components/VerticalBoxSlot.h"

#include "SidheRigel/SidheRigelPlayerController.h"
#include "SidheRigel/InGameMapScriptActor.h"
#include "SidheRigel/UI/InGameUI.h"
#include "SidheRigel/UI/CharacterStatus.h"
#include "SidheRigel/UI/SkillBtn.h"
#include "SidheRigel/UI/StatSummary.h"
#include "Components/WidgetComponent.h"
#include "SidheRigel/Minion/Minion.h"
#include "SidheRigel/Tower/Tower.h"
#include "SidheRigel/UI/TalentUI.h"
#include "SidheRigel/UI/TalentItem.h"

ASidheRigelCharacter::ASidheRigelCharacter()
{
	// Set size for player capsule
	GetCapsuleComponent()->InitCapsuleSize(42.f, 96.0f);

	// Don't rotate character to camera direction
	bUseControllerRotationPitch = false;
	bUseControllerRotationYaw = false;
	bUseControllerRotationRoll = false;

	// Configure character movement
	GetCharacterMovement()->bOrientRotationToMovement = true; // Rotate character to moving direction
	GetCharacterMovement()->RotationRate = FRotator(0.f, 640.f, 0.f);
	GetCharacterMovement()->bConstrainToPlane = true;
	GetCharacterMovement()->bSnapToPlaneAtStart = true;

	// Create a camera boom...
	CameraBoom = CreateDefaultSubobject<USpringArmComponent>(TEXT("CameraBoom"));
	CameraBoom->SetupAttachment(RootComponent);
	CameraBoom->SetUsingAbsoluteRotation(true); // Don't want arm to rotate when character does
	CameraBoom->TargetArmLength = 800.f;
	CameraBoom->SetRelativeRotation(FRotator(-60.f, 0.f, 0.f));
	CameraBoom->bDoCollisionTest = false; // Don't want to pull camera in when it collides with level

	// Create a camera...
	TopDownCameraComponent = CreateDefaultSubobject<UCameraComponent>(TEXT("TopDownCamera"));
	TopDownCameraComponent->SetupAttachment(CameraBoom, USpringArmComponent::SocketName);
	TopDownCameraComponent->bUsePawnControlRotation = false; // Camera does not rotate relative to arm

	// Create Can Detect Range
	detectRange = CreateDefaultSubobject<USphereComponent>(TEXT("DetectRange"));
	detectRange->InitSphereRadius(500.0f);
	detectRange->SetupAttachment(RootComponent);

	// Create Skill Range
	skillRange = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("SkillRange"));
	static ConstructorHelpers::FObjectFinder<UStaticMesh>SkillRangeMesh(TEXT("/Game/Dummy/SM_AssetPlatform"));
	if (SkillRangeMesh.Succeeded())
	{
		skillRange->SetStaticMesh(SkillRangeMesh.Object);
	}
	skillRange->SetRelativeLocation(FVector(0, 0, -100));
	skillRange->SetVisibility(false);
	skillRange->SetCollisionProfileName(UCollisionProfile::NoCollision_ProfileName);
	skillRange->SetupAttachment(RootComponent);

	// Activate ticking in order to update the cursor every frame.
	PrimaryActorTick.bCanEverTick = true;
	PrimaryActorTick.bStartWithTickEnabled = true;

	InitAttackProjectile();
	//GEngine->AddOnScreenDebugMessage(-1, 2, FColor::Red, FString::Printf(TEXT("DEBUG")));

	//StatWidget
	InitStatWidget();

	// 7X3 Array
	for (int i = 0; i < 7; i++)
	{
		FIsSelectedTalentItem item;
		for (int j = 0; j < 3; j++)
		{
			item.Add(false);
		}

		IsSelectedTalent.Add(item);
	}

	InitTalentLIst();

	//Init Talent Widget Subclass
	InitTalentWidget();
}

void ASidheRigelCharacter::BeginPlay()
{
	Super::BeginPlay();
	InitProperty();

	AInGameMapScriptActor* LevelScriptActor = Cast<AInGameMapScriptActor>(GetWorld()->GetLevelScriptActor());

	InGameUI = LevelScriptActor->InGameUI;

	InGameUI->CharacterStatus->InitCharacterStatus(this);
	
	InitStatSummary();

	

	UE_LOG(LogTemp, Warning, TEXT("Character BeginPlay"));

	detectRange->OnComponentBeginOverlap.AddDynamic(this, &ASidheRigelCharacter::OnEnterEnemy);
	detectRange->OnComponentEndOverlap.AddDynamic(this, &ASidheRigelCharacter::OnExitEnemy);

	GetWorldTimerManager().SetTimer(GenerateHPTimer, this, &ASidheRigelCharacter::IE_GenerateHP, 1.f, true);

	DisplayTalentList(0);
}

void ASidheRigelCharacter::Tick(float DeltaSeconds)
{
    Super::Tick(DeltaSeconds);

	if (IsMoveVectorTrue)
	{
		FVector Location = GetActorLocation();
		Location += moveDirection * (moveForce/10) * DeltaSeconds;
		SetActorLocation(Location);
		moveCnt++;
		if (moveCnt <= 10)
		{
			moveDirection = FVector::ZeroVector;
			moveForce = 0;
			moveCnt = 0;

			IsMoveVectorTrue = false;
		}
	}
}

void ASidheRigelCharacter::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	check(PlayerInputComponent);

}

void ASidheRigelCharacter::OnEnterEnemy(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{
	if (ITeam* TeamEnemy = Cast<ITeam>(OtherActor))
	{
		if (TeamEnemy->GetTeam() != GetTeam())
		{
			if (AMinion* MinionEnemy = Cast<AMinion>(OtherActor))
			{
				InRangeActors.Add(OtherActor);
			}
			else if (ATower* TowerEnemy = Cast<ATower>(OtherActor))
			{
				InRangeActors.Add(OtherActor);
			}
		}
	}
}

void ASidheRigelCharacter::OnExitEnemy(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex)
{
	InRangeActors.Remove(OtherActor);
}

void ASidheRigelCharacter::ChangeTarget()
{
	for (auto Enemy : InRangeActors)
	{
		if (AMinion* MinionEnemy = Cast<AMinion>(Enemy))
		{
			MinionEnemy->currentTarget = this;
		}
		else if (ATower* TowerEnemy = Cast<ATower>(Enemy))
		{
			TowerEnemy->currentTarget = this;
		}
	}
}

void ASidheRigelCharacter::UseSkill(FHitResult HitResult, E_SkillState SkillState)
{
	UE_LOG(LogTemp, Warning, TEXT("UseSkill"));
}

void ASidheRigelCharacter::InitTalentLIst()
{
	//Talent Init
	for (int i = 0; i < 7; i++)
	{
		FTalentList tmp;
		talentListArray.Add(tmp);

		int iItemCount;
		if (i == 3)	//if level 10 Talent
		{
			//Ultimate Skill Count
			iItemCount = 1;
		}
		else if (i == 6)//if level 20 Talent
		{
			iItemCount = 3;
		}
		else
		{
			iItemCount = 3;
		}

		talentListArray[i].itemCount = iItemCount;
		for (int j = 0; j < iItemCount; j++)
		{
			FTalent tmpTalent = FTalent{"", "", ""};
			talentListArray[i].talentItems.Add(tmpTalent);
		}
	}
}

void ASidheRigelCharacter::RemoveTalentUI(int32 Index)
{
	UTalentUI* tmp = TalentUIArray[Index];
	InGameUI->TalentUIOverlay->RemoveChild(tmp);
	TalentUIArray.Remove(Index);
}

void ASidheRigelCharacter::InitTalentWidget()
{
	ConstructorHelpers::FClassFinder<UUserWidget> TalentUIClass(TEXT("/Game/UIBlueprints/InGameUI/WBP_TalentUI"));
	if (TalentUIClass.Class == nullptr)
		return;

	TalentUIWidget = TalentUIClass.Class;

	ConstructorHelpers::FClassFinder<UUserWidget> TalentItemClass(TEXT("/Game/UIBlueprints/InGameUI/WBP_TalentItem"));
	if (TalentItemClass.Class == nullptr)
		return;

	TalentItemWidget = TalentItemClass.Class;

}

void ASidheRigelCharacter::DisplayTalentList(int32 Index)
{
	UTalentUI* talentUI = CreateWidget<UTalentUI>(InGameUI, TalentUIWidget);

	for (int i = 0; i < talentListArray[Index].itemCount; i++)
	{
		UTalentItem* talentItem = CreateWidget<UTalentItem>(InGameUI, TalentItemWidget);
		talentItem->InitTalentItemRef(this, Index, i);

		talentUI->TalentItemList.Add(talentItem);
		talentUI->TalentItemBox->AddChild(talentItem);
	}

	InGameUI->TalentUIOverlay->AddChild(talentUI);
	TalentUIArray.Add(Index, talentUI);

	UOverlaySlot* slot = Cast<UOverlaySlot>(talentUI->Slot);
	
	if (slot)
	{
		slot->SetHorizontalAlignment(EHorizontalAlignment::HAlign_Fill);
		slot->SetVerticalAlignment(EVerticalAlignment::VAlign_Bottom);
	}
}

void ASidheRigelCharacter::InitStatWidget()
{
	StatWidget = CreateDefaultSubobject<UWidgetComponent>(TEXT("StatWIDGET"));
	StatWidget->SetupAttachment(GetMesh());

	StatWidget->SetRelativeLocation(FVector(0, 0, 240));
	StatWidget->SetWidgetSpace(EWidgetSpace::Screen);
	static ConstructorHelpers::FClassFinder<UUserWidget> StatUI(TEXT("/Game/UIBlueprints/InGameUI/WBP_StatSummary"));
	if (StatUI.Succeeded())
	{
		StatWidget->SetWidgetClass(StatUI.Class);
		StatWidget->SetDrawSize(FVector2D(150, 30));
	}
}

void ASidheRigelCharacter::InitStatSummary()
{
	auto TmpWidget = Cast<UStatSummary>(StatWidget->GetUserWidgetObject());
	if (nullptr != TmpWidget)
	{
		StatSummary = TmpWidget;
		StatSummary->InitStat();
	}
}

void ASidheRigelCharacter::SetUISkillCoolDown(E_SkillState SkillState, float Percentage, float CurrentCoolDown)
{
	InGameUI->CharacterStatus->SkillButtons[SkillState]->SetCoolDownProgress(Percentage, CurrentCoolDown);
}

void ASidheRigelCharacter::ClearUISkillCoolDown(E_SkillState SkillState)
{
	InGameUI->CharacterStatus->SkillButtons[SkillState]->ClearCoolDownProgress();
}

void ASidheRigelCharacter::SetLevel(int32 _level)
{
	level = _level;

	if ((level != 19) && (level % 3 == 1 || level == 20))
	{
		
	}
}

int32 ASidheRigelCharacter::GetCurrentLevel()
{
	return level;
}

void ASidheRigelCharacter::SetCurrentHP(float _hp)
{
	currentHP = _hp;

	float var_MaxHP = GetMaxHP();
	if (currentHP > var_MaxHP)
	{
		currentHP = var_MaxHP;
	}

	InGameUI->CharacterStatus->UpdateHP();
	StatSummary->SetHPBar(currentHP / GetMaxHP());

}

float ASidheRigelCharacter::GetCurrentHP()
{
	return currentHP;
}

void ASidheRigelCharacter::IE_GenerateHP()
{
	RestoreHP(GetGenerateHealthPoint());
}

void ASidheRigelCharacter::UseMana(float UseMP)
{
	currentMP -= UseMP;
	if (currentMP < 0)
		currentMP = 0;

	InGameUI->CharacterStatus->UpdateMP();
}

float ASidheRigelCharacter::GetCurrentMP()
{
	return currentMP;
}

int32 ASidheRigelCharacter::GetMoney()
{
	return money;
}

void ASidheRigelCharacter::GiveMoney(int32 _money)
{
	money += _money;

	InGameUI->CharacterStatus->UpdateMoney();
}

int32 ASidheRigelCharacter::GetExp()
{
	return experience;
}

void ASidheRigelCharacter::GiveExp(int32 _exp)
{
	experience += _exp;
	if (experience >= MaxExperience)
	{
		experience -= MaxExperience;
		level++;
		int32 idx = 0;
		if (level == 4)
		{
			idx = 1;
		}
		if (level == 7)
		{
			idx = 2;
		}
		if (level == 10)
		{
			idx = 3;
		}
		if (level == 13)
		{
			idx = 4;
		}
		if (level == 16)
		{
			idx = 5;
		}
		if (level == 20)
		{
			idx = 6;
		}

		if (idx != 0)
		{
			DisplayTalentList(idx);
		}
	}

	InGameUI->CharacterStatus->UpdateLevel();
	StatSummary->SetLevel(level);
	StatSummary->SetExpBar(float(experience) / MaxExperience);
}

int32 ASidheRigelCharacter::GetMaxExp()
{
	return MaxExperience;
}

float ASidheRigelCharacter::GetRange()
{
	float res = 0;
	for (auto& value : range)
	{
		res += value.Value;
	}

	return res;
}

float ASidheRigelCharacter::GetAttackDamage()
{
	float res = 0;
	for (auto& value : attackDamage)
	{
		res += value.Value;
	}

	return res;
}

int32 ASidheRigelCharacter::GetCriticalRate()
{
	int32 res = 0;
	for (auto& value : criticalRate)
	{
		res += value.Value;
	}
	if (res > 100)	//MaxCriticalRate
	{
		res = 100;
	}

	return res;
}

int32 ASidheRigelCharacter::GetCriticalDamage()
{
	int32 res = 0;
	for (auto& value : criticalDamage)
	{
		res += value.Value;
	}
	
	return res;
}

float ASidheRigelCharacter::GetAttackSpeed()
{
	float res = 0;

	for (auto& value : attackSpeed)
	{
		res += value.Value;
	}
	if (res > 2.5f)	//MaxAttackSpeed
	{
		res = 2.5f;
	}

	return res;
}

void ASidheRigelCharacter::AddAttackSpeed(FString name, float value)
{
	attackSpeed.Add(name, value);
}

void ASidheRigelCharacter::RemoveAttackSpeed(FString name)
{
	attackSpeed.Remove(name);
}

float ASidheRigelCharacter::GetMaxHP()
{
	float res = 0;
	for (auto& value : MaxHP)
	{
		res += value.Value;
	}

	return res;
}

float ASidheRigelCharacter::GetGenerateHealthPoint()
{
	float res = 0;
	for (auto& value : generateHealthPoint)
	{
		res += value.Value;
	}

	return res;
}

float ASidheRigelCharacter::GetMaxMP()
{
	float res = 0;
	for (auto& value : MaxMP)
	{
		res += value.Value;
	}

	return res;
}

int32 ASidheRigelCharacter::GetLifeSteal()
{
	int32 res = 0;
	for (auto& value : lifeSteal)
	{
		res += value.Value;
	}

	return res;
}

int32 ASidheRigelCharacter::GetProtectPower()
{
	int32 res = 0;
	for (auto& value : protectPower)
	{
		res += value.Value;
	}

	return res;
}

void ASidheRigelCharacter::AddDefencePoint(FString name, float value, float time)
{
	if (defencePoint.Contains(name))
		return;

	defencePoint.Add(name, value);

	if (time == -1)
		return;

	FTimerHandle buffTimer;
	GetWorldTimerManager().SetTimer(buffTimer, FTimerDelegate::CreateLambda([=]()
		{
			if (defencePoint.Find(name))
			{
				defencePoint.Remove(name);
			}
		})
		, time, false);
}

float ASidheRigelCharacter::GetDefencePoint()
{
	float res = 0.f;
	for (auto& value : defencePoint)
	{
		res += value.Value;
	}

	res *= (1 - GetDecreseDefence());

	return res;
}

void ASidheRigelCharacter::AddSpeed(FString name, float value, float time)
{
	if (speed.Contains(name))
		return;

	speed.Add(name, value);

	if (time == -1)
		return;

	FTimerHandle buffTimer;
	GetWorldTimerManager().SetTimer(buffTimer, FTimerDelegate::CreateLambda([=]()
		{
			if (speed.Find(name))
			{
				speed.Remove(name);
			}
		})
		, time, false);
}

float ASidheRigelCharacter::GetSpeed()
{
	float res = 0.f;
	for (auto& value : speed)
	{
		res += value.Value;
	}

	for (auto& slowRate : speedRate)
	{
		res *= (1- slowRate.Value);
	}

	return res;
}

void ASidheRigelCharacter::AddDecreseDefencePercent(FString name, float value, float time)
{
	if (decreseDefencePoint.Contains(name))
		return;

	decreseDefencePoint.Add(name, value);

	if (time == -1)
		return;

	FTimerHandle DebuffTimer;
	GetWorldTimerManager().SetTimer(DebuffTimer, FTimerDelegate::CreateLambda([=]()
		{
			if (decreseDefencePoint.Find(name))
			{
				decreseDefencePoint.Remove(name);
			}
		})
		, time, false);
}

float ASidheRigelCharacter::GetDecreseDefence()
{
	float res = 0.f;
	for (auto& value : decreseDefencePoint)
	{
		res += value.Value;
	}

	return res;
}

float ASidheRigelCharacter::GetRemainDieCooldown()
{
	float res = 0;

	auto myController = Cast<ASidheRigelPlayerController>(GetController());
	if (myController)
	{
		res = myController->stateMachine->DieTime;
	}
	return res;
}

void ASidheRigelCharacter::InitProperty()
{
	level = 1;
	range.Add("Debug", 500.f);
	attackDamage.Add("Debug", 5.f);
	attackSpeed.Add("Debug", 1.f);
	criticalRate.Add("Debug", 50);
	criticalDamage.Add("Debug", 50);

	MaxHP.Add("Debug", 100.f);
	generateHealthPoint.Add("Debug", 0.2f);
	MaxMP.Add("Debug", 100.f);
	lifeSteal.Add("Debug", 5.f);
	protectPower.Add("Debug", 20);

	defencePoint.Add("Debug", 100);

	speed.Add("Debug", 600.f);

	currentHP = GetMaxHP();
	currentMP = GetMaxMP();

	MaxExperience = 20;
}

void ASidheRigelCharacter::InitAttackProjectile()
{
	static ConstructorHelpers::FObjectFinder<UBlueprint> Projectile(TEXT("/Game/Dummy/BP_DummyProjectile"));
	if (Projectile.Object)
	{
		baseProjectileClass = (UClass*)Projectile.Object->GeneratedClass;
	}
}

void ASidheRigelCharacter::Attack(AActor* target)
{
	if (baseProjectileClass)
	{
		FVector MuzzleLocation = GetActorLocation();
		FRotator MuzzleRotation = GetActorRotation();

		UWorld* World = GetWorld();
		if (World)
		{
			FActorSpawnParameters SpawnParams;
			SpawnParams.Owner = this;
			SpawnParams.Instigator = GetInstigator();

			// Spawn the projectile at the muzzle.
			ADummyProjectile* Projectile = World->SpawnActor<ADummyProjectile>(baseProjectileClass, MuzzleLocation, MuzzleRotation, SpawnParams);
			if (Projectile)
			{
				Projectile->Target = target;
				InitProjectileProperty(Projectile);

				if (ASidheRigelCharacter* characterTarget = Cast<ASidheRigelCharacter>(target))
				{
					ChangeTarget();
				}
			}
		}
	}
}

void ASidheRigelCharacter::InitProjectileProperty(ADummyProjectile* projectile)
{
	projectile->projectileOwner = this;
	projectile->AttackDamage = GetAttackDamage();
	projectile->criticalRate = (float)GetCriticalRate() / 100.f;
	projectile->criticalDamage = (float)GetCriticalDamage() / 100.f + 1;
}

void ASidheRigelCharacter::LifeSteal(float damage)
{
	float _lifeSteal = (float)GetLifeSteal() / 100.f;
	
	RestoreHP(damage * _lifeSteal);
}

void ASidheRigelCharacter::Stun(float time)
{
	ASidheRigelPlayerController* controller = Cast<ASidheRigelPlayerController>(GetController());
	if (controller && controller->stateMachine)
	{
		controller->stateMachine->OnStun(time);
	}
}

void ASidheRigelCharacter::Stop(float time)
{
	ASidheRigelPlayerController* controller = Cast<ASidheRigelPlayerController>(GetController());
	if (controller && controller->stateMachine)
	{
		controller->stateMachine->OnStop(time);
	}
}

void ASidheRigelCharacter::Slow(float time, float value, FString key)
{
	if (speedRate.Contains(key))
	{
		return;
	}

	speedRate.Add(key, value);

	if (time == -1)
		return;

	FTimerHandle SlowTimer;
	GetWorldTimerManager().SetTimer(SlowTimer, FTimerDelegate::CreateLambda([=]()
		{
			if (speedRate.Find(key))
			{
				speedRate.Remove(key);
			}
		})
		, time, false);
}

void ASidheRigelCharacter::Silence(float time)
{
	ASidheRigelPlayerController* controller = Cast<ASidheRigelPlayerController>(GetController());
	if (controller && controller->stateMachine)
	{
		controller->stateMachine->OnSilence(time);
	}
}

void ASidheRigelCharacter::TakeDamage(float damage, AActor* damageCauser)
{
	currentHP -= damage;
	UE_LOG(LogTemp, Warning, TEXT("CurrentHP : %f"), currentHP);
	if (ASidheRigelCharacter* causerCharacter = Cast<ASidheRigelCharacter>(damageCauser))
	{
		causerCharacter->LifeSteal(damage);
	}
	if (currentHP <= 0)
	{
		UE_LOG(LogTemp, Warning, TEXT("Die"));
		currentHP = 0;
	}

	InGameUI->CharacterStatus->UpdateHP();
	StatSummary->SetHPBar(currentHP / GetMaxHP());
}

void ASidheRigelCharacter::RestoreHP(float value)
{
	currentHP += value;
	
	float var_MaxHP = GetMaxHP();

	if (currentHP > var_MaxHP)
	{
		currentHP = var_MaxHP;
	}

	InGameUI->CharacterStatus->UpdateHP();
	StatSummary->SetHPBar(currentHP / GetMaxHP());
}

float ASidheRigelCharacter::GetHP()
{
	return currentHP;
}

void ASidheRigelCharacter::MoveVector(FVector Direction, float Force)
{
	auto unitDirection = (Direction * FVector(1, 1, 0)).GetSafeNormal();
	LaunchCharacter(unitDirection * Force, false, false);
}

E_Team ASidheRigelCharacter::GetTeam()
{
	return team;
}
