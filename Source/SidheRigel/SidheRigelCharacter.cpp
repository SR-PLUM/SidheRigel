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
#include "SidheRigel/Character/Common/StunParticle.h"
#include "SidheRigel/Character/Common/SlowParticle.h"
#include "SidheRigel/Character/Common/StopParticle.h"
#include "SidheRigel/Character/Common/SilenceParticle.h"

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

	/*
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
	*/

	// Make sure camera won't respond to collision with the player character
	GetCapsuleComponent()->SetCollisionResponseToChannel(ECollisionChannel::ECC_Camera, ECollisionResponse::ECR_Ignore);
	GetMesh()->SetCollisionResponseToChannel(ECollisionChannel::ECC_Camera, ECollisionResponse::ECR_Ignore);

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
	sidheRigelController = Cast<ASidheRigelPlayerController>(GetController());

	if (sidheRigelController)
	{
		FInputModeGameAndUI InputModeData;
		InputModeData.SetLockMouseToViewportBehavior(EMouseLockMode::LockAlways);
		InputModeData.SetHideCursorDuringCapture(false);

		sidheRigelController->SetInputMode(InputModeData);
	}

	InitProperty();

	AInGameMapScriptActor* LevelScriptActor = Cast<AInGameMapScriptActor>(GetWorld()->GetLevelScriptActor());

	InGameUI = LevelScriptActor->InGameUI;

	InGameUI->CharacterStatus->InitCharacterStatus(this);
	
	InitStatSummary();

	detectRange->OnComponentBeginOverlap.AddDynamic(this, &ASidheRigelCharacter::OnEnterEnemy);
	detectRange->OnComponentEndOverlap.AddDynamic(this, &ASidheRigelCharacter::OnExitEnemy);

	GetWorldTimerManager().SetTimer(GenerateHPTimer, this, &ASidheRigelCharacter::IE_GenerateHP, 1.f, true);

	DisplayTalentList(0);

	GetWorldTimerManager().SetTimer(stateMachineTimer, this, &ASidheRigelCharacter::CustomTick, 0.05f, true);
}

void ASidheRigelCharacter::Tick(float DeltaSeconds)
{
    Super::Tick(DeltaSeconds);

	//치유감소
	if (reduceHealDuration > 0)
	{
		reduceHealDuration -= DeltaSeconds;
		if (reduceHealDuration <= 0)
		{
			reduceMyHeal = 0;
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

void ASidheRigelCharacter::SpawnStunParticle()
{
	if (stunParticle)
		return;

	UWorld* World = GetWorld();
	if (World)
	{
		FActorSpawnParameters SpawnParams;
		FTransform SpawnTransform;
		SpawnTransform.SetLocation(GetActorLocation() + FVector::UpVector * 100);
		SpawnTransform.SetRotation(GetActorRotation().Quaternion());
		SpawnParams.Owner = this;
		SpawnParams.SpawnCollisionHandlingOverride = ESpawnActorCollisionHandlingMethod::AlwaysSpawn;

		// Spawn the projectile at the muzzle.
		stunParticle = World->SpawnActorDeferred<AStunParticle>(stunParticleClass, SpawnTransform);
		
		if (stunParticle)
		{
			stunParticle->target = this;
		}

		stunParticle->FinishSpawning(SpawnTransform);
	}
}

void ASidheRigelCharacter::RemoveStunParticle()
{
	if (!stunParticle)
		return;

	stunParticle->Destroy();
	stunParticle = nullptr;
}

void ASidheRigelCharacter::SpawnSlowParticle()
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

void ASidheRigelCharacter::RemoveSlowParticle()
{
	if (!slowParticle)
		return;

	slowParticle->Destroy();
	slowParticle = nullptr;
}

void ASidheRigelCharacter::SpawnStopParticle()
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

void ASidheRigelCharacter::RemoveStopParticle()
{
	if (!stopParticle)
		return;

	stopParticle->Destroy();
	stopParticle = nullptr;
}

void ASidheRigelCharacter::SpawnSilenceParticle()
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

void ASidheRigelCharacter::RemoveSilenceParticle()
{
	if (!silenceParticle)
		return;

	silenceParticle->Destroy();
	silenceParticle = nullptr;
}

void ASidheRigelCharacter::SetLevel(int32 _level)
{
	level = _level;
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

	//InGameUI->CharacterStatus->UpdateMoney();
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

void ASidheRigelCharacter::AddAttackDamage(FString name, float value)
{
	attackDamage.Add(name, value);
}

void ASidheRigelCharacter::RemoveAttackDamage(FString name)
{
	attackDamage.Remove(name);
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

	if (res > 100)
	{
		res = 100;
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

int32 ASidheRigelCharacter::GetEndurance()
{
	int32 res = 0;
	for (auto& value : endurance)
	{
		res += value.Value;
	}

	if (res > 80)
	{
		res = 80;
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

void ASidheRigelCharacter::SetReduceMyHeal(int32 reduceHeal, float duration)
{
	if (reduceMyHeal <= reduceHeal)
	{
		reduceMyHeal = reduceHeal;

		reduceHealDuration = duration;
	}
}

FReduceHeal ASidheRigelCharacter::GetReduceOtherHeal()
{
	FReduceHeal res;
	res.debuffAmout = 0;
	res.debuffDuration = 0;

	for (auto& item : reduceOtherHeal)
	{
		if (item.Value.debuffAmout > res.debuffAmout)
		{
			res = item.Value;
		}
		else if (item.Value.debuffAmout == res.debuffAmout)
		{
			if (item.Value.debuffDuration > res.debuffDuration)
			{
				res = item.Value;
			}
		}
	}

	return res;
}

float ASidheRigelCharacter::GetRemainDieCooldown()
{
	float res = 0;

	if (sidheRigelController)
	{
		res = sidheRigelController->stateMachine->DieTime;
	}
	return res;
}

void ASidheRigelCharacter::AddBarrierAmount(float value)
{
	float tmp = barrierAmount;
	GetWorldTimerManager().ClearTimer(BarrierTimer);

	barrierAmount = tmp + value;

	GetWorldTimerManager().SetTimer(BarrierTimer, FTimerDelegate::CreateLambda([=]()
		{
			barrierAmount = 0.f;
		})
		, barrierDuration, false);
}

void ASidheRigelCharacter::DecreaseBarrierAmount(float value)
{
	barrierAmount -= value;

	if (barrierAmount < 0)
	{
		barrierAmount = 0.f;
	}
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



void ASidheRigelCharacter::RemoveReduceOtherHeal(FString name)
{
	reduceOtherHeal.Remove(name);
}



void ASidheRigelCharacter::AddReduceOtherHeal(FString name, int32 reduceHeal, float duration)
{
	FReduceHeal tmp;
	tmp.debuffAmout = reduceHeal;
	tmp.debuffDuration = duration;

	reduceOtherHeal.Add(name, tmp);
}

void ASidheRigelCharacter::LifeSteal(float damage)
{
	float _lifeSteal = (float)GetLifeSteal() / 100.f;
	
	RestoreHP(damage * _lifeSteal);
}

void ASidheRigelCharacter::CustomTick()
{
	if (sidheRigelController && sidheRigelController->stateMachine)
	{
		sidheRigelController->stateMachine->Update();
	}
}

void ASidheRigelCharacter::Stun(float time)
{
	if (sidheRigelController && sidheRigelController->stateMachine)
	{
		float totalTime = time * (1 - (GetEndurance() / 100.f));
		sidheRigelController->stateMachine->OnStun(totalTime);
	}
}

void ASidheRigelCharacter::Stop(float time)
{
	if (sidheRigelController && sidheRigelController->stateMachine)
	{
		float totalTime = time * (1 - (GetEndurance() / 100.f));
		sidheRigelController->stateMachine->OnStop(totalTime);
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

	float totalTime = (1 - (GetEndurance() / 100.f)) * time;

	SpawnSlowParticle();

	FTimerHandle SlowTimer;	//잘 동작하는지 확인하기
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
		, totalTime, false);
}

void ASidheRigelCharacter::Silence(float time)
{
	if (sidheRigelController && sidheRigelController->stateMachine)
	{
		float totalTime = time * (1 - (GetEndurance() / 100.f));
		sidheRigelController->stateMachine->OnSilence(totalTime);
	}
}

void ASidheRigelCharacter::TakeDamage(float damage, AActor* damageCauser)
{
	float tmp = damage - barrierAmount;
	DecreaseBarrierAmount(damage);

	currentHP -= tmp;

	if (ASidheRigelCharacter* causerCharacter = Cast<ASidheRigelCharacter>(damageCauser))
	{
		//치유감소 설정
		auto reduceHeal = causerCharacter->GetReduceOtherHeal();
		SetReduceMyHeal(reduceHeal.debuffAmout, reduceHeal.debuffDuration);

		//흡혈
		causerCharacter->LifeSteal(damage);
	}
	if (currentHP <= 0)
	{
		currentHP = 0;
	}

	InGameUI->CharacterStatus->UpdateHP();
	StatSummary->SetHPBar(currentHP / GetMaxHP());
}

void ASidheRigelCharacter::RestoreHP(float value)
{
	float restoreHP = value * (1 - reduceMyHeal / 100);
	currentHP += restoreHP;
	
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
