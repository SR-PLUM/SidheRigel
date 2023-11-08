// Fill out your copyright notice in the Description page of Project Settings.


#include "FairyWingCharacter.h"
#include "GameFramework/SpringArmComponent.h"
#include "Components/SphereComponent.h"

#include "SidheRigel/Character/FairyWing/Skill/FairyWingQCollider.h"
#include "SidheRigel/Character/FairyWing/Skill/FairyWingQSkill.h"
#include "SidheRigel/Character/FairyWing/Skill/FairyWingWCollider.h"
#include "SidheRigel/Character/FairyWing/Skill/FairyWingWSkill.h"
#include "SidheRigel/Character/FairyWing/Skill/FairyWingRCollider.h"
#include "SidheRigel/Character/FairyWing/Skill/FairyWingRSkill.h"
#include "SidheRigel/Character/FairyWing/Skill/FairyWingEProjectile.h"
#include "SidheRigel/Character/FairyWing/Skill/FairyWingESkill.h"
#include "SidheRigel/Character/FairyWing/FairyWingAttackProjectile.h"
#include "SidheRigel/InGameMapScriptActor.h"

// Sets default values
AFairyWingCharacter::AFairyWingCharacter()
{
	static ConstructorHelpers::FObjectFinder<UBlueprint> EProjectile(TEXT("/Game/Heros/FairyWing/Skill/BP_FairyWingEProjectile"));
	if (EProjectile.Object)
	{
		EProjectileClass = (UClass*)EProjectile.Object->GeneratedClass;
	}

	static ConstructorHelpers::FObjectFinder<UBlueprint> QCollider(TEXT("/Game/Heros/FairyWing/Skill/BP_FairyWingQCollider"));
	if (QCollider.Object)
	{
		QColliderClass = (UClass*)QCollider.Object->GeneratedClass;
	}

	static ConstructorHelpers::FObjectFinder<UBlueprint> WCollider(TEXT("/Game/Heros/FairyWing/Skill/BP_FairyWingWCollider"));
	if (WCollider.Object)
	{
		WColliderClass = (UClass*)WCollider.Object->GeneratedClass;
	}

	static ConstructorHelpers::FObjectFinder<UBlueprint> RCollider(TEXT("/Game/Heros/FairyWing/Skill/BP_FairyWingRCollider"));
	if (RCollider.Object)
	{
		RColliderClass = (UClass*)RCollider.Object->GeneratedClass;
	}

	InitAttackProjectile();

	InitFairyWingTalent();
}

// Called when the game starts or when spawned
void AFairyWingCharacter::BeginPlay()
{
	Super::BeginPlay();
	
	skills.Add({ E_SkillState::Q_Ready, NewObject<UFairyWingQSkill>() });
	if (skills[E_SkillState::Q_Ready] != nullptr)
	{
		skills[E_SkillState::Q_Ready]->SetSkillProperty(this, E_SkillState::Q_Ready);
	}
	skills.Add({ E_SkillState::W_Ready, NewObject<UFairyWingWSkill>() });
	if (skills[E_SkillState::W_Ready] != nullptr)
	{
		skills[E_SkillState::W_Ready]->SetSkillProperty(this, E_SkillState::W_Ready);
	}
	skills.Add({ E_SkillState::E_Ready, NewObject<UFairyWingESkill>() });
	if (skills[E_SkillState::E_Ready] != nullptr)
	{
		skills[E_SkillState::E_Ready]->SetSkillProperty(this, E_SkillState::E_Ready);
	}
	skills.Add({ E_SkillState::R_Ready, NewObject<UFairyWingRSkill>() });
	if (skills[E_SkillState::R_Ready] != nullptr)
	{
		skills[E_SkillState::R_Ready]->SetSkillProperty(this, E_SkillState::R_Ready);
	}
}

// Called every frame
void AFairyWingCharacter::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

// Called to bind functionality to input
void AFairyWingCharacter::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);

}

void AFairyWingCharacter::InitAttackProjectile()
{
	static ConstructorHelpers::FObjectFinder<UBlueprint> Projectile(TEXT("/Game/Heros/FairyWing/BP_FairyWingAttackProjectile"));
	if (Projectile.Object)
	{
		attackProjectileClass = (UClass*)Projectile.Object->GeneratedClass;
	}
}

void AFairyWingCharacter::Attack(AActor* target)
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
			AFairyWingAttackProjectile* Projectile = World->SpawnActor<AFairyWingAttackProjectile>(attackProjectileClass, MuzzleLocation, MuzzleRotation, SpawnParams);
			if (Projectile)
			{
				Projectile->Target = target;
				InitProjectileProperty(Projectile);
			}
		}
	}
}

void AFairyWingCharacter::InitFairyWingTalent()
{
	//level 1
	talentListArray[0].talentItems[0].talentName = "FairyWing_1_1";
	talentListArray[0].talentItems[0].talentDescription = "Q 장판 지속시간 증가";
	talentListArray[0].talentItems[0].imgPath = "";

	talentListArray[0].talentItems[1].talentName = "FairyWing_1_2";
	talentListArray[0].talentItems[1].talentDescription = "W 데미지 증가";
	talentListArray[0].talentItems[1].imgPath = "";

	talentListArray[0].talentItems[2].talentName = "FairyWing_1_3";
	talentListArray[0].talentItems[2].talentDescription = "E 맞은사람 시야 제공";
	talentListArray[0].talentItems[2].imgPath = "";

	//level 4
	talentListArray[1].talentItems[0].talentName = "FairyWing_2_1";
	talentListArray[1].talentItems[0].talentDescription = "Q 아군 치유";
	talentListArray[1].talentItems[0].imgPath = "";

	talentListArray[1].talentItems[1].talentName = "FairyWing_2_2";
	talentListArray[1].talentItems[1].talentDescription = "Q 적에게 디버프(실명)";
	talentListArray[1].talentItems[1].imgPath = "";

	talentListArray[1].talentItems[2].talentName = "FairyWing_2_3";
	talentListArray[1].talentItems[2].talentDescription = "Q 데미지 증가";
	talentListArray[1].talentItems[2].imgPath = "";

	//level 7
	talentListArray[2].talentItems[0].talentName = "FairyWing_3_1";
	talentListArray[2].talentItems[0].talentDescription = "W 마지막 공격 피흡";
	talentListArray[2].talentItems[0].imgPath = "";

	talentListArray[2].talentItems[1].talentName = "FairyWing_3_2";
	talentListArray[2].talentItems[1].talentDescription = "Q와 W를 동시에 맞추면 속박";
	talentListArray[2].talentItems[1].imgPath = "";

	talentListArray[2].talentItems[2].talentName = "FairyWing_3_3";
	talentListArray[2].talentItems[2].talentDescription = "E 같은 대상에게 E 연속으로 맞추면 속박";
	talentListArray[2].talentItems[2].imgPath = "";

	//level 10
	talentListArray[3].talentItems[0].talentName = "FairyWing_4_1";
	talentListArray[3].talentItems[0].talentDescription = "R 침묵 추가 (일시적)";
	talentListArray[3].talentItems[0].imgPath = "";

	//level 13
	talentListArray[4].talentItems[0].talentName = "FairyWing_5_1";
	talentListArray[4].talentItems[0].talentDescription = "W 시야 제거";
	talentListArray[4].talentItems[0].imgPath = "";

	talentListArray[4].talentItems[1].talentName = "FairyWing_5_2";
	talentListArray[4].talentItems[1].talentDescription = "E 아군에게 사용시 치유";
	talentListArray[4].talentItems[1].imgPath = "";

	talentListArray[4].talentItems[2].talentName = "FairyWing_5_3";
	talentListArray[4].talentItems[2].talentDescription = "Q 장판 면적 증가";
	talentListArray[4].talentItems[2].imgPath = "";

	//level 16
	talentListArray[5].talentItems[0].talentName = "FairyWing_6_1";
	talentListArray[5].talentItems[0].talentDescription = "E 맞춘 대상에게 표식을 남겨 다른 스킬로 터트리기";
	talentListArray[5].talentItems[0].imgPath = "";

	talentListArray[5].talentItems[1].talentName = "FairyWing_6_2";
	talentListArray[5].talentItems[1].talentDescription = "Q 아군 공격속도 증가 버프";
	talentListArray[5].talentItems[1].imgPath = "";

	talentListArray[5].talentItems[2].talentName = "FairyWing_6_3";
	talentListArray[5].talentItems[2].talentDescription = "E 다음 일반공격 강화";
	talentListArray[5].talentItems[2].imgPath = "";

	//level 20
	talentListArray[6].talentItems[0].talentName = "FairyWing_7_1";
	talentListArray[6].talentItems[0].talentDescription = "R 중앙으로 끌어당김";
	talentListArray[6].talentItems[0].imgPath = "";

	talentListArray[6].talentItems[1].talentName = "FairyWing_7_2";
	talentListArray[6].talentItems[1].talentDescription = "W 마지막 공격의 데미지 감소 및 상시 적용";
	talentListArray[6].talentItems[1].imgPath = "";

	talentListArray[6].talentItems[2].talentName = "FairyWing_7_3";
	talentListArray[6].talentItems[2].talentDescription = "E 영웅에게 적중시 쿨타임 감소";
	talentListArray[6].talentItems[2].imgPath = "";
}
