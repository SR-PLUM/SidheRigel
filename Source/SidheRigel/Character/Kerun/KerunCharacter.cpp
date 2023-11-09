// Fill out your copyright notice in the Description page of Project Settings.

#include "KerunCharacter.h"
#include "../../Dummy/DummyProjectile.h"

#include "Skills/KerunQSkill.h"
#include "Skills/KerunWSkill.h"
#include "Skills/KerunR1Skill.h"
#include "Skills/KerunESkillTalentQuest.h"

#include "KerunAttackProjectile.h"
#include "KerunAnimInstance.h"

#include "GameFramework/ProjectileMovementComponent.h"
#include "GameFramework/CharacterMovementComponent.h"

AKerunCharacter::AKerunCharacter()
{
	InitAttackProjectile();

	ProjectileMovementComponent = CreateDefaultSubobject<UProjectileMovementComponent>(TEXT("ProjectileMovementComponent"));

	InitKerunTalent();
}

void AKerunCharacter::BeginPlay()
{
	Super::BeginPlay();

	//AnimInstance = Cast<UKerunAnimInstance>(GetMesh()->GetAnimInstance());
	
	skills.Add({ E_SkillState::Q_Ready, NewObject<UKerunQSkill>() });
	if (skills[E_SkillState::Q_Ready] != nullptr)
	{
		skills[E_SkillState::Q_Ready]->SetSkillProperty(this, E_SkillState::Q_Ready);
	}
	
	skills.Add({ E_SkillState::W_Ready, NewObject<UKerunWSkill>() });
	if (skills[E_SkillState::W_Ready] != nullptr)
	{
		skills[E_SkillState::W_Ready]->SetSkillProperty(this, E_SkillState::W_Ready);
	}
	/*
	skills.Add({ E_SkillState::E_Ready, new ColdESkill });
	if (skills[E_SkillState::E_Ready] != nullptr)
	{
		skills[E_SkillState::E_Ready]->SetSkillProperty(this, E_SkillState::E_Ready);
	}*/
	skills.Add({ E_SkillState::R_Ready, NewObject<UKerunR1Skill>()});
	if (skills[E_SkillState::R_Ready] != nullptr)
	{
		skills[E_SkillState::R_Ready]->SetSkillProperty(this, E_SkillState::R_Ready);
	}

	ESkillTalentQuest = NewObject<UKerunESkillTalentQuest>();
	ESkillTalentQuest->InitTalentQuestProperty(this);
}

void AKerunCharacter::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
}

void AKerunCharacter::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);
}

void AKerunCharacter::InitProperty()
{
	level = 3;
	range.Add("Debug", 200.f);
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

void AKerunCharacter::Attack(AActor* target)
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
		AKerunAttackProjectile* Projectile = World->SpawnActor<AKerunAttackProjectile>(ProjectileClass, MuzzleLocation, MuzzleRotation, SpawnParams);
		if (Projectile)
		{
			//Kerun QSkill
			UKerunQSkill* QSkillRef = Cast<UKerunQSkill>(skills[E_SkillState::Q_Ready]);

			if (QSkillRef->IsWorking)
			{
				QSkillRef->AttackCount += 1;
			}

			// Set the projectile's initial trajectory.
			Projectile->Target = target;
			InitProjectileProperty(Projectile);

			if (IsSelectedTalent[0][2])
			{
				ADummyProjectile* TmpProjectile = Cast<ADummyProjectile>(Projectile);

				TmpProjectile->AttackDamage = GetAttackDamage() + QSkillRef->GetQDamage();
			}

			ImproveEStack(EAddAmount);

			QSkillRef->ApplyTalentWhenFullComboHits(target);
			
		}
	}

	/*
	if (AnimInstance)
	{
		AnimInstance->PlayAttackMontage();
		UE_LOG(LogTemp, Warning, TEXT("AnimInstance is Not Null"));
	}
	*/
}

void AKerunCharacter::InitAttackProjectile()
{
	static ConstructorHelpers::FObjectFinder<UBlueprint> Projectile(TEXT("/Game/Heros/Kerun/BP_KerunAttackProjectile"));
	if (Projectile.Object)
	{
		ProjectileClass = (UClass*)Projectile.Object->GeneratedClass;
	}
}

void AKerunCharacter::SetCurrentHP(float _hp)
{
	Super::SetCurrentHP(_hp);

	/*
	if (KerunR2SkillRef->GetIsWorking())
	{
		if (KerunR2SkillRef->CheckCurrentHP(currentHP))
		{
			currentHP = KerunR2SkillRef->GetMinHP();
		}
	}
	*/
}

void AKerunCharacter::ImproveEStack(int Count)
{

	ECurrentStack += Count;
	
	if (ECurrentStack > EMaxStack)
	{
		ECurrentStack = EMaxStack;
		if (IsSelectedTalent[2][0])
		{
			ESkillTalentQuest->OnCompleted();
		}
	}

	UE_LOG(LogTemp, Warning, TEXT("EStack :: %d"), ECurrentStack);
	StartETimer();
}

void AKerunCharacter::SetEMaxStack(int32 Value)
{
	EMaxStack = Value;
}

void AKerunCharacter::SetEAddAmount(int32 Value)
{
	EAddAmount = Value;
}

void AKerunCharacter::InitKerunTalent()
{
	//level 1
	talentListArray[0].talentItems[0].talentName = "Kerun_1_1";
	talentListArray[0].talentItems[0].talentDescription = "Q 퀘스트 : 3번의 강화공격 모두 맞추기 10번 \n 보상 : 강화공격이 5번으로 증가";
	talentListArray[0].talentItems[0].imgPath = "";

	talentListArray[0].talentItems[1].talentName = "Kerun_1_2";
	talentListArray[0].talentItems[1].talentDescription = "Q 모두 맞추면 방어력 감소";
	talentListArray[0].talentItems[1].imgPath = "";

	talentListArray[0].talentItems[2].talentName = "Kerun_1_3";
	talentListArray[0].talentItems[2].talentDescription = "Q 강화 공격이 점점 강해짐";
	talentListArray[0].talentItems[2].imgPath = "";

	//level 4
	talentListArray[1].talentItems[0].talentName = "Kerun_2_1";
	talentListArray[1].talentItems[0].talentDescription = "W 퀘스트 : 막타치기 20회 \n 보상 : W 사용시 재생의 구슬 5개 획득";
	talentListArray[1].talentItems[0].imgPath = "";

	talentListArray[1].talentItems[1].talentName = "Kerun_2_2";
	talentListArray[1].talentItems[1].talentDescription = "W 둔화 추가";
	talentListArray[1].talentItems[1].imgPath = "";

	talentListArray[1].talentItems[2].talentName = "Kerun_2_3";
	talentListArray[1].talentItems[2].talentDescription = "W 보호막 추가";
	talentListArray[1].talentItems[2].imgPath = "";

	//level 7
	talentListArray[2].talentItems[0].talentName = "Kerun_3_1";
	talentListArray[2].talentItems[0].talentDescription = "E 퀘스트 : 재생의 구슬 최대치를 달성하기 \n 보상 : 재생의 구슬 최대치를 35로 증가시키고 재생의 구슬이 2배로 쌓임";
	talentListArray[2].talentItems[0].imgPath = "";

	talentListArray[2].talentItems[1].talentName = "Kerun_3_2";
	talentListArray[2].talentItems[1].talentDescription = "E 체력 재생량 증가";
	talentListArray[2].talentItems[1].imgPath = "";

	talentListArray[2].talentItems[2].talentName = "Kerun_3_3";
	talentListArray[2].talentItems[2].talentDescription = "E 스택 유지 시간 증가";
	talentListArray[2].talentItems[2].imgPath = "";

	//level 10
	talentListArray[3].talentItems[0].talentName = "Kerun_4_1";
	talentListArray[3].talentItems[0].talentDescription = "R 선택한 적 영웅을 기절시키고, 생명의 구슬 6개 획득";
	talentListArray[3].talentItems[0].imgPath = "";

	//level 13
	talentListArray[4].talentItems[0].talentName = "Kerun_4_1";
	talentListArray[4].talentItems[0].talentDescription = "패시브 흡혈 15%";
	talentListArray[4].talentItems[0].imgPath = "";

	talentListArray[4].talentItems[1].talentName = "Kerun_4_2";
	talentListArray[4].talentItems[1].talentDescription = "패시브 인내력 20%";
	talentListArray[4].talentItems[1].imgPath = "";

	talentListArray[4].talentItems[2].talentName = "Kerun_5_3";
	talentListArray[4].talentItems[2].talentDescription = "패시브 상대방 방향으로 이동할 때 이동속도 증가";
	talentListArray[4].talentItems[2].imgPath = "";

	//level 16
	talentListArray[5].talentItems[0].talentName = "Kerun_6_1";
	talentListArray[5].talentItems[0].talentDescription = "Q 확정 치명타 피해";
	talentListArray[5].talentItems[0].imgPath = "";

	talentListArray[5].talentItems[1].talentName = "Kerun_6_2";
	talentListArray[5].talentItems[1].talentDescription = "W 치유 감소 효과 적용";
	talentListArray[5].talentItems[1].imgPath = "";

	talentListArray[5].talentItems[2].talentName = "Kerun_6_3";
	talentListArray[5].talentItems[2].talentDescription = "W 아군한테 사용 가능";
	talentListArray[5].talentItems[2].imgPath = "";

	//level 20
	talentListArray[6].talentItems[0].talentName = "Kerun_7_1";
	talentListArray[6].talentItems[0].talentDescription = "패시브 일반공격이 주변 적에게도 100%의 피해";
	talentListArray[6].talentItems[0].imgPath = "";

	talentListArray[6].talentItems[1].talentName = "Kerun_7_2";
	talentListArray[6].talentItems[1].talentDescription = "Q 사용 중 받는 피해 30% 감소";
	talentListArray[6].talentItems[1].imgPath = "";

	talentListArray[6].talentItems[2].talentName = "Kerun_7_3";
	talentListArray[6].talentItems[2].talentDescription = "R 범위 내의 모든 적 영웅을 둔화, 밖으로 밀어내기";
	talentListArray[6].talentItems[2].imgPath = "";
}

float AKerunCharacter::GetEDuration()
{
	if (IsSelectedTalent[2][2])
	{
		return UpgradeEDuration;
	}
	else
	{
		return EDuration;
	}
}

float AKerunCharacter::GetEHealthRate()
{
	if (IsSelectedTalent[2][1])
	{
		return UpgradeEHealthRate;
	}
	else
	{
		return EHealthRate;
	}
}

void AKerunCharacter::StartETimer()
{
	QuitETimer();

	float GenerateAmount = ECurrentStack * GetEHealthRate();

	generateHealthPoint.Add("ESkill", GenerateAmount);

	GetWorld()->GetTimerManager().SetTimer(
		ETimer,
		FTimerDelegate::CreateLambda([&]() {
			QuitETimer();
			ECurrentStack = 0;
			UE_LOG(LogTemp, Warning, TEXT("ECurrentStack Initialized"));
			}),
		GetEDuration(),
		false
		);
}

void AKerunCharacter::QuitETimer()
{
	GetWorldTimerManager().ClearTimer(ETimer);
	generateHealthPoint.Remove("ESkill");
}
