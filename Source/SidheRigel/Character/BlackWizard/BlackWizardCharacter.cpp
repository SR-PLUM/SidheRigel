#include "BlackWizardCharacter.h"
#include "Components/SphereComponent.h"
#include "SidheRigel/Character/BlackWizard/Skill/BlackWizardQCollider.h"
#include "SidheRigel/Character/BlackWizard/Skill/BlackWizardQSkill.h"
#include "SidheRigel/Character/BlackWizard/Skill/BlackWizardWSkill.h"
#include "SidheRigel/Character/BlackWizard/Skill/BlackWizardECollider.h"
#include "SidheRigel/Character/BlackWizard/Skill/BlackWizardESkill.h"
#include "SidheRigel/Character/BlackWizard/Skill/BlackWizardRCollider.h"
#include "SidheRigel/Character/BlackWizard/Skill/BlackWizardRSkill.h"
#include "SidheRigel/Character/BlackWizard/BlackWizardAttackProjectile.h"

// Sets default values
ABlackWizardCharacter::ABlackWizardCharacter()
{
 	// Set this character to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	//PrimaryActorTick.bCanEverTick = true;

	static ConstructorHelpers::FObjectFinder<UBlueprint> RCollider(TEXT("/Game/Heros/BlackWizard/Skill/BP_BlackWizardRCollider"));
	if (RCollider.Object)
	{
		RColliderClass = (UClass*)RCollider.Object->GeneratedClass;
	}

	static ConstructorHelpers::FObjectFinder<UBlueprint> QCollider(TEXT("/Game/Heros/BlackWizard/Skill/BP_BlackWizardQCollider"));
	if (QCollider.Object)
	{
		QColliderClass = (UClass*)QCollider.Object->GeneratedClass;
	}

	static ConstructorHelpers::FObjectFinder<UBlueprint> ECollider(TEXT("/Game/Heros/BlackWizard/Skill/BP_BlackWizardECollider"));
	if (ECollider.Object)
	{
		EColliderClass = (UClass*)ECollider.Object->GeneratedClass;
	}

	InitAttackProjectile();

	skills.Add({ E_SkillState::Q_Ready, new BlackWizardQSkill });
	if (skills[E_SkillState::Q_Ready] != nullptr)
	{
		skills[E_SkillState::Q_Ready]->SetSkillProperty(this, E_SkillState::Q_Ready);
	}
	skills.Add({ E_SkillState::W_Ready, new BlackWizardWSkill });
	if (skills[E_SkillState::W_Ready] != nullptr)
	{
		skills[E_SkillState::W_Ready]->SetSkillProperty(this, E_SkillState::W_Ready);
	}
	skills.Add({ E_SkillState::E_Ready, new BlackWizardESkill });
	if (skills[E_SkillState::E_Ready] != nullptr)
	{
		skills[E_SkillState::E_Ready]->SetSkillProperty(this, E_SkillState::E_Ready);
	}
	skills.Add({ E_SkillState::R_Ready, new BlackWizardRSkill });
	if (skills[E_SkillState::R_Ready] != nullptr)
	{
		skills[E_SkillState::R_Ready]->SetSkillProperty(this, E_SkillState::R_Ready);
	}
}

// Called when the game starts or when spawned
void ABlackWizardCharacter::BeginPlay()
{
	Super::BeginPlay();
	
}

// Called every frame
void ABlackWizardCharacter::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

// Called to bind functionality to input
void ABlackWizardCharacter::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);

}

void ABlackWizardCharacter::InitAttackProjectile()
{
	static ConstructorHelpers::FObjectFinder<UBlueprint> Projectile(TEXT("/Game/Heros/BlackWizard/BP_BlackWizardAttackProjectile"));
	if (Projectile.Object)
	{
		attackProjectileClass = (UClass*)Projectile.Object->GeneratedClass;
	}
}

void ABlackWizardCharacter::Attack(AActor* target)
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
			ABlackWizardAttackProjectile* Projectile = World->SpawnActor<ABlackWizardAttackProjectile>(attackProjectileClass, MuzzleLocation, MuzzleRotation, SpawnParams);
			if (Projectile)
			{
				Projectile->Target = target;
				InitProjectileProperty(Projectile);
			}
		}
	}
}

void ABlackWizardCharacter::InitBlackWizardTalent()
{
	//level 1
	talentListArray[0].talentItems[0].talentName = "BlackWizard_1_1";
	talentListArray[0].talentItems[0].talentDescription = "Q 적 막타를 치면 소모한 마나를 돌려받음";
	talentListArray[0].talentItems[0].imgPath = "";

	talentListArray[0].talentItems[1].talentName = "BlackWizard_1_2";
	talentListArray[0].talentItems[1].talentDescription = "W 방어력 증가";
	talentListArray[0].talentItems[1].imgPath = "";

	talentListArray[0].talentItems[2].talentName = "BlackWizard_1_3";
	talentListArray[0].talentItems[2].talentDescription = "E 범위증가";
	talentListArray[0].talentItems[2].imgPath = "";

	//level 4
	talentListArray[1].talentItems[0].talentName = "BlackWizard_2_1";
	talentListArray[1].talentItems[0].talentDescription = "W 돌진 도착 지점의 적 기절";
	talentListArray[1].talentItems[0].imgPath = "";

	talentListArray[1].talentItems[1].talentName = "BlackWizard_2_2";
	talentListArray[1].talentItems[1].talentDescription = "W 돌진 경로의 적 기절";
	talentListArray[1].talentItems[1].imgPath = "";

	talentListArray[1].talentItems[2].talentName = "BlackWizard_2_3";
	talentListArray[1].talentItems[2].talentDescription = "W 돌진 경로 데미지";
	talentListArray[1].talentItems[2].imgPath = "";

	//level 7
	talentListArray[2].talentItems[0].talentName = "BlackWizard_3_1";
	talentListArray[2].talentItems[0].talentDescription = "Q 데미지증가";
	talentListArray[2].talentItems[0].imgPath = "";

	talentListArray[2].talentItems[1].talentName = "BlackWizard_3_2";
	talentListArray[2].talentItems[1].talentDescription = "W 돌진거리증가";
	talentListArray[2].talentItems[1].imgPath = "";

	talentListArray[2].talentItems[2].talentName = "BlackWizard_3_3";
	talentListArray[2].talentItems[2].talentDescription = "E 중심부 적에게 기절";
	talentListArray[2].talentItems[2].imgPath = "";

	//level 10
	talentListArray[3].talentItems[0].talentName = "BlackWizard_4_1";
	talentListArray[3].talentItems[0].talentDescription = "R 적을 부채꼴모양으로 날림";
	talentListArray[3].talentItems[0].imgPath = "";

	//level 13
	talentListArray[4].talentItems[0].talentName = "BlackWizard_4_1";
	talentListArray[4].talentItems[0].talentDescription = "E 내가 상태이상에 걸리면 쿨타임 초기화 (10초)";
	talentListArray[4].talentItems[0].imgPath = "";

	talentListArray[4].talentItems[1].talentName = "BlackWizard_4_2";
	talentListArray[4].talentItems[1].talentDescription = "W 보호막 생성";
	talentListArray[4].talentItems[1].imgPath = "";

	talentListArray[4].talentItems[2].talentName = "BlackWizard_5_3";
	talentListArray[4].talentItems[2].talentDescription = "Q 피흡";
	talentListArray[4].talentItems[2].imgPath = "";

	//level 16
	talentListArray[5].talentItems[0].talentName = "BlackWizard_6_1";
	talentListArray[5].talentItems[0].talentDescription = "Q 쿨타임증가, 맞출때 마다 데미지 증가 및 쿨타임 고정감소";
	talentListArray[5].talentItems[0].imgPath = "";

	talentListArray[5].talentItems[1].talentName = "BlackWizard_6_2";
	talentListArray[5].talentItems[1].talentDescription = "W 적에게 맞추면 쿨타임 초기화 (1번)";
	talentListArray[5].talentItems[1].imgPath = "";

	talentListArray[5].talentItems[2].talentName = "BlackWizard_6_3";
	talentListArray[5].talentItems[2].talentDescription = "E 즉발로 변경";
	talentListArray[5].talentItems[2].imgPath = "";

	//level 20
	talentListArray[6].talentItems[0].talentName = "BlackWizard_7_1";
	talentListArray[6].talentItems[0].talentDescription = "R 사거리 증가 및 주변 영웅 치유";
	talentListArray[6].talentItems[0].imgPath = "";

	talentListArray[6].talentItems[1].talentName = "BlackWizard_7_2";
	talentListArray[6].talentItems[1].talentDescription = "Q 스택에 비례해 공격력 상승";
	talentListArray[6].talentItems[1].imgPath = "";

	talentListArray[6].talentItems[2].talentName = "BlackWizard_7_3";
	talentListArray[6].talentItems[2].talentDescription = "E 속박후 수면";
	talentListArray[6].talentItems[2].imgPath = "";
}
