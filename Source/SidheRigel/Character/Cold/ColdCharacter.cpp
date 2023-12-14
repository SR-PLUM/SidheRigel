// Fill out your copyright notice in the Description page of Project Settings.


#include "ColdCharacter.h"
#include "GameFramework/SpringArmComponent.h"

#include "SidheRigel/Character/Cold/Skill/ColdQSkill.h"
#include "SidheRigel/Character/Cold/Skill/ColdWSkill.h"
#include "SidheRigel/Character/Cold/Skill/ColdESkill.h"
#include "SidheRigel/Character/Cold/Skill/ColdR1Skill.h"
#include "SidheRigel/Character/Cold/Skill/ColdR2Skill.h"
#include "SidheRigel/Character/Cold/Skill/ColdWProjectile.h"
#include "SidheRigel/Character/Cold/Skill/ColdEProjectile.h"
#include "SidheRigel/Character/Cold/Skill/ColdEDamageField.h"
#include "SidheRigel/Character/Cold/Skill/ColdR1Projectile.h"
#include "SidheRigel/Character/Cold/Skill/ColdR2Projectile.h"
#include "SidheRigel/Character/Cold/ColdAttackProjectile.h"
#include "SidheRigel/InGameMapScriptActor.h"
#include "SidheRigel/UI/StatSummary.h"

// Sets default values
AColdCharacter::AColdCharacter()
{
	for (int i = 0; i < 5; i++)
	{
		QMuzzle.Add(CreateDefaultSubobject<UStaticMeshComponent>(TEXT("Muzzle ") + i));
		QMuzzle[i]->SetupAttachment(GetMesh());
	}

	InitAttackProjectile();

	InitColdTalent();
}

// Called when the game starts or when spawned
void AColdCharacter::BeginPlay()
{
	Super::BeginPlay();
	
	skills.Add({ E_SkillState::Q_Ready, NewObject<UColdQSkill>() });
	if (skills[E_SkillState::Q_Ready] != nullptr)
	{
		skills[E_SkillState::Q_Ready]->SetSkillProperty(this, E_SkillState::Q_Ready);
	}
	skills.Add({ E_SkillState::W_Ready, NewObject<UColdWSkill>() });
	if (skills[E_SkillState::W_Ready] != nullptr)
	{
		skills[E_SkillState::W_Ready]->SetSkillProperty(this, E_SkillState::W_Ready);
	}
	skills.Add({ E_SkillState::E_Ready, NewObject<UColdESkill>() });
	if (skills[E_SkillState::E_Ready] != nullptr)
	{
		skills[E_SkillState::E_Ready]->SetSkillProperty(this, E_SkillState::E_Ready);
	}
	skills.Add({ E_SkillState::R_Ready, NewObject<UColdR2Skill>() });
	if (skills[E_SkillState::R_Ready] != nullptr)
	{
		skills[E_SkillState::R_Ready]->SetSkillProperty(this, E_SkillState::R_Ready);
	}

	StatSummary->SetHPBar(currentHP / GetMaxHP());
	StatSummary->SetMPBar(currentMP / GetMaxMP());
	StatSummary->SetLevel(level);
	StatSummary->SetExpBar(float(experience) / MaxExperience);
}

// Called every frame
void AColdCharacter::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
}

// Called to bind functionality to input
void AColdCharacter::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);

}

void AColdCharacter::InitAttackProjectile()
{
	static ConstructorHelpers::FObjectFinder<UClass> Projectile(TEXT("Blueprint'/Game/Heros/Cold/BP_ColdAttackProjectile.BP_ColdAttackProjectile_C'"));
	if (Projectile.Object)
	{
		attackProjectileClass = (UClass*)Projectile.Object;
	}
}

void AColdCharacter::Attack(AActor* target)
{
	if (attackProjectileClass)
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
			AColdAttackProjectile* Projectile = World->SpawnActor<AColdAttackProjectile>(attackProjectileClass, MuzzleLocation, MuzzleRotation, SpawnParams);
			if (Projectile)
			{
				Projectile->Target = target;
				InitProjectileProperty(Projectile);
			}
		}
	}
}

void AColdCharacter::InitProperty()
{
	level = 9;

	MaxExperience = 280;

	rangeBase = 600;
	attackDamageBase = 60;
	attackSpeedBase = 0.568;
	criticalRateBase = 0;
	criticalDamageBase = 75;

	MaxHPBase = 580;
	generateHealthPointBase = 3.5;
	MaxMPBase = 315;
	generateManaPointBase = 7.4;

	lifeStealBase = 0;
	protectPowerBase = 0;
	defencePointBase = 27;

	speedBase = 325;

	currentHP = GetMaxHP();
	currentMP = GetMaxMP();
}

void AColdCharacter::LevelUp()
{
	Super::LevelUp();

	MaxHPBase += 107;
	currentHP += 107;
	generateHealthPointBase += 0.55;
	MaxMPBase += 40;
	currentMP += 40;
	generateManaPointBase += 0.7;
	attackDamageBase += 3.4;
	attackSpeedBase += 0.028;
	defencePointBase += 4.7;

	StatSummary->SetHPBar(currentHP / GetMaxHP());
	StatSummary->SetMPBar(currentMP / GetMaxMP());
}

void AColdCharacter::InitColdTalent()
{
	//level 1
	talentListArray[0].talentItems[0].talentName = "Cold_1_1";
	talentListArray[0].talentItems[0].talentDescription = "Q 유도탄 사거리 증가";
	talentListArray[0].talentItems[0].imgPath = "/Game/Heros/Cold/TalentIcons/Cold_1_1";

	talentListArray[0].talentItems[1].talentName = "Cold_1_2";
	talentListArray[0].talentItems[1].talentDescription = "W 피흡";
	talentListArray[0].talentItems[1].imgPath = "/Game/Heros/Cold/TalentIcons/Cold_1_2";

	talentListArray[0].talentItems[2].talentName = "Cold_1_3";
	talentListArray[0].talentItems[2].talentDescription = "E 소모 마나량 증가 및 데미지 증가";
	talentListArray[0].talentItems[2].imgPath = "/Game/Heros/Cold/TalentIcons/Cold_1_3";

	//level 4
	talentListArray[1].talentItems[0].talentName = "Cold_2_1";
	talentListArray[1].talentItems[0].talentDescription = "W 밀어내고 벽생성";
	talentListArray[1].talentItems[0].imgPath = "/Game/Heros/Cold/TalentIcons/Cold_2_1";

	talentListArray[1].talentItems[1].talentName = "Cold_2_2";
	talentListArray[1].talentItems[1].talentDescription = "W 밀어내고 방어력 증가";
	talentListArray[1].talentItems[1].imgPath = "/Game/Heros/Cold/TalentIcons/Cold_2_2";

	talentListArray[1].talentItems[2].talentName = "Cold_2_3";
	talentListArray[1].talentItems[2].talentDescription = "W 밀어내고 이속증가";
	talentListArray[1].talentItems[2].imgPath = "/Game/Heros/Cold/TalentIcons/Cold_2_3";

	//level 7
	talentListArray[2].talentItems[0].talentName = "Cold_3_1";
	talentListArray[2].talentItems[0].talentDescription = "E 범위증가";
	talentListArray[2].talentItems[0].imgPath = "/Game/Heros/Cold/TalentIcons/Cold_3_1";

	talentListArray[2].talentItems[1].talentName = "Cold_3_2";
	talentListArray[2].talentItems[1].talentDescription = "E 사거리증가";
	talentListArray[2].talentItems[1].imgPath = "/Game/Heros/Cold/TalentIcons/Cold_3_2";

	talentListArray[2].talentItems[2].talentName = "Cold_3_3";
	talentListArray[2].talentItems[2].talentDescription = "E 후딜레이감소";
	talentListArray[2].talentItems[2].imgPath = "/Game/Heros/Cold/TalentIcons/Cold_3_3";

	//level 10
	talentListArray[3].talentItems[0].talentName = "Cold_4_1";
	talentListArray[3].talentItems[0].talentDescription = "R 사거리가 굉장히 긴 논타겟팅 레이저 발사";
	talentListArray[3].talentItems[0].imgPath = "/Game/Heros/Cold/TalentIcons/Cold_4_1";

	//level 13
	talentListArray[4].talentItems[0].talentName = "Cold_5_1";
	talentListArray[4].talentItems[0].talentDescription = "Q 유도탄 갯수 증가";
	talentListArray[4].talentItems[0].imgPath = "/Game/Heros/Cold/TalentIcons/Cold_5_1";

	talentListArray[4].talentItems[1].talentName = "Cold_5_2";
	talentListArray[4].talentItems[1].talentDescription = "Q 유도탄 1타 이후 데미지도 100%";
	talentListArray[4].talentItems[1].imgPath = "/Game/Heros/Cold/TalentIcons/Cold_5_2";

	talentListArray[4].talentItems[2].talentName = "Cold_5_3";
	talentListArray[4].talentItems[2].talentDescription = "Q 유도탄 사용시 이동속도 증가";
	talentListArray[4].talentItems[2].imgPath = "/Game/Heros/Cold/TalentIcons/Cold_5_3";

	//level 16
	talentListArray[5].talentItems[0].talentName = "Cold_6_1";
	talentListArray[5].talentItems[0].talentDescription = "Q 근처 적에게도 유도탄 발사";
	talentListArray[5].talentItems[0].imgPath = "/Game/Heros/Cold/TalentIcons/Cold_6_1";

	talentListArray[5].talentItems[1].talentName = "Cold_6_2";
	talentListArray[5].talentItems[1].talentDescription = "Q 슬로우";
	talentListArray[5].talentItems[1].imgPath = "/Game/Heros/Cold/TalentIcons/Cold_6_2";

	talentListArray[5].talentItems[2].talentName = "Cold_6_3";
	talentListArray[5].talentItems[2].talentDescription = "E 피흡";
	talentListArray[5].talentItems[2].imgPath = "/Game/Heros/Cold/TalentIcons/Cold_6_3";

	//level 20
	talentListArray[6].talentItems[0].talentName = "Cold_7_1";
	talentListArray[6].talentItems[0].talentDescription = "W 나도 뒤로 이동 (벽넘기 가능)";
	talentListArray[6].talentItems[0].imgPath = "/Game/Heros/Cold/TalentIcons/Cold_7_1";

	talentListArray[6].talentItems[1].talentName = "Cold_7_2";
	talentListArray[6].talentItems[1].talentDescription = "E 처형";
	talentListArray[6].talentItems[1].imgPath = "/Game/Heros/Cold/TalentIcons/Cold_7_2";

	talentListArray[6].talentItems[2].talentName = "Cold_7_3";
	talentListArray[6].talentItems[2].talentDescription = "R2 사거리가 무제한";
	talentListArray[6].talentItems[2].imgPath = "/Game/Heros/Cold/TalentIcons/Cold_7_3";
}

void AColdCharacter::PlayQSkillSound()
{
	if (QSkillSound)
	{
		UGameplayStatics::PlaySoundAtLocation(this, QSkillSound, this->GetActorLocation());
	}
}

void AColdCharacter::PlayWSkillSound()
{
	if (WSkillSound)
	{
		UGameplayStatics::PlaySoundAtLocation(this, WSkillSound, this->GetActorLocation());
	}
}

void AColdCharacter::PlayESkillSound()
{
	if (ESkillSound)
	{
		UGameplayStatics::PlaySoundAtLocation(this, ESkillSound, this->GetActorLocation());
	}
}

void AColdCharacter::PlayRSkillSound()
{
	if (RSkillSound)
	{
		UGameplayStatics::PlaySoundAtLocation(this, RSkillSound, this->GetActorLocation());
	}
}

