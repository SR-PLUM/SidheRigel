#include "BlackWizardCharacter.h"
#include "Components/SphereComponent.h"
#include "SidheRigel/Character/BlackWizard/Skill/BlackWizardQCollider.h"
#include "SidheRigel/Character/BlackWizard/Skill/BlackWizardQSkill.h"
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
	/*skills.Add({ E_SkillState::W_Ready, new BlackWizardWSkill });
	if (skills[E_SkillState::W_Ready] != nullptr)
	{
		skills[E_SkillState::W_Ready]->SetSkillProperty(this);
	}*/
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