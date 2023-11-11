// Fill out your copyright notice in the Description page of Project Settings.


#include "KerunR1Skill.h"

#include "../KerunCharacter.h"

#include "SidheRigel/Interface/Damagable.h"
#include "SidheRigel/Interface/CCable.h"

#include "KerunR1SkillTalentCollider.h"

UKerunR1Skill::UKerunR1Skill()
{
	static ConstructorHelpers::FObjectFinder<UBlueprint>colliderRef(TEXT("/Game/Heros/Kerun/Skills/BP_R1SkillTalentCollider"));
	if (colliderRef.Object)
	{
		colliderClass = (UClass*)colliderRef.Object->GeneratedClass;
	}
}

void UKerunR1Skill::SetSkillProperty(ASidheRigelCharacter* Character, E_SkillState SkillState)
{
	skillDelay = 1.f;
	skillCooldown = 0.f;
	skillMaxCooldown = 20.f;
	range = 300.f;

	bIsInstantCast = false;
	bIsTargeting = true;

	character = Character;
	skillstate = SkillState;

	character->GetWorldTimerManager().SetTimer(cooldownTimer, this, &USkill::OnTick, 0.1f, true);
}

void UKerunR1Skill::OnUse(FHitResult Hit)
{
	if (AActor* Actor = Hit.GetActor())
	{
		if (IDamagable* Target = Cast<IDamagable>(Actor))
		{
			Target->TakeDamage(15.0f, character);

			if (ICCable* CCTarget = Cast<ICCable>(Actor))
			{
				CCTarget->Stun(2.0f);
			}

			AKerunCharacter* KerunCharacter = Cast<AKerunCharacter>(character);

			KerunCharacter->ImproveEStack(6);

			if (KerunCharacter->IsSelectedTalent[6][2])
			{
				UWorld* world = character->GetWorld();
				if (world)
				{
					FActorSpawnParameters SpawnParams;
					FTransform SpawnTransform;
					SpawnTransform.SetLocation(character->GetActorLocation());
					SpawnTransform.SetRotation(character->GetActorRotation().Quaternion());
					SpawnParams.Owner = character;
					SpawnParams.SpawnCollisionHandlingOverride = ESpawnActorCollisionHandlingMethod::AlwaysSpawn;

					
					AKerunR1SkillTalentCollider* collider = world->SpawnActorDeferred<AKerunR1SkillTalentCollider>(colliderClass, SpawnTransform);
					if (collider)
					{
						collider->colliderOwner = character;
					}
					collider->FinishSpawning(SpawnTransform);

				}
			}
		}
	}
}
