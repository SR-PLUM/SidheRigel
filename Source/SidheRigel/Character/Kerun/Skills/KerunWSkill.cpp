// Fill out your copyright notice in the Description page of Project Settings.


#include "KerunWSkill.h"
#include "../KerunCharacter.h"
#include "SidheRigel/Interface/Damagable.h"
#include "GameFramework/ProjectileMovementComponent.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "Kismet/GameplayStatics.h"
#include "Kismet/GameplayStaticsTypes.h"
#include "KerunWSkillTalentQuest.h"
#include "SidheRigel/SidheRigelPlayerController.h"
#include "Engine/EngineTypes.h"
#include "Components/CapsuleComponent.h"

UKerunWSkill::UKerunWSkill()
{
	WSkillTalentQuest = NewObject<UKerunWSkillTalentQuest>();
	WSkillTalentQuest->Initialize();
}

void UKerunWSkill::SetSkillProperty(ASidheRigelCharacter* Character, E_SkillState SkillState)
{
	skillDelay = 1.f;
	skillCooldown = 0.f;
	skillMaxCooldown = 2.f;
	range = 500.f;

	bIsInstantCast = false;
	bIsTargeting = true;

	character = Character;
	skillstate = SkillState;

	KerunCharacter = Cast<AKerunCharacter>(character);

	character->GetWorldTimerManager().SetTimer(cooldownTimer, this, &UKerunWSkill::OnTick, 0.1f, true);
}

void UKerunWSkill::OnTick()
{
	Super::OnTick();

	//Kerun WSkill
	if (GetIsWorking() && !IsLanding)
	{
		FVector Loc = character->GetActorLocation();

		if (Loc.Z >= GetLimitZValue())
		{
			LandingIntoTarget(KerunCharacter);
			IsLanding = true;

			KerunCharacter->PlayWSkillEndMontage();
		}

	}

	if (IsLanding)
	{
		FVector Loc = character->GetActorLocation();

		if (Loc.Z <= LandingZValue)
		{
			KnockDownTarget(KerunCharacter);
			
			IsLanding = false;
		}
	}
}

void UKerunWSkill::OnUse(FHitResult Hit)
{
	if (AActor* Actor = Hit.GetActor())
	{
		if (IDamagable* Target = Cast<IDamagable>(Actor))
		{
			if (AKerunCharacter* Kerun = Cast<AKerunCharacter>(character))
			{
				Kerun->PlayWSkillSound();
			}

			JumpIntoTarget(Actor);

			KerunCharacter->ImproveEStack(3);
		}
	}
}

bool UKerunWSkill::CanUse()
{
	if (!bIsTargeting)
	{
		return true;
	}

	auto SRController = Cast<ASidheRigelPlayerController>(character->GetController());
	if (SRController)
	{
		auto hit = SRController->GetHitResult();

		if (character->GetDistanceTo(hit.GetActor()) > range)
			return false;

		auto teamActor = Cast<ITeam>(hit.GetActor());
		if (teamActor && teamActor->GetTeam() != character->GetTeam())
			return true;

		if (teamActor && teamActor->GetTeam() == character->GetTeam() && character->IsSelectedTalent[5][2])
			return true;

	}

	return false;
}

void UKerunWSkill::JumpIntoTarget(AActor* Actor)
{
	IsWorking = true;
	TargetActor = Actor;
	// Move Owner
	FVector StartLoc = character->GetActorLocation();
	FVector EndLoc = Actor->GetActorLocation();
	TargetLocation = EndLoc;
	FVector Velocity = (EndLoc - StartLoc) *0.2f;
	PrevVelocity = StartLoc-EndLoc;
	Velocity.Z = StartLoc.Z + ZValue;
	Velocity *= Speed - 1;

	character->GetCharacterMovement()->Launch(Velocity);

	KerunCharacter->PlayWSkillStartMontage();
}

bool UKerunWSkill::GetIsWorking()
{
	return IsWorking;
}

void UKerunWSkill::SetIsWorking(bool flag)
{
	IsWorking = flag;
}

void UKerunWSkill::KnockDownTarget(AKerunCharacter* Owner)
{
	if (character->IsSelectedTalent[5][2])
	{
		if (ITeam* team = Cast<ITeam>(TargetActor))
		{
			if (team->GetTeam() != Cast<ITeam>(character)->GetTeam())
			{
				AttackTarget(TargetActor);
			}
		}
	}
	else
	{
		AttackTarget(TargetActor);
	}

	Owner->GetCapsuleComponent()->SetCollisionProfileName(FName("Pawn"));
	Owner->UseParticleSystem(W_Ready);
}

double UKerunWSkill::GetLimitZValue()
{
	return TargetLocation.Z + ZValue;
}

void UKerunWSkill::AttackTarget(AActor* Actor)
{
	if (IDamagable* Target = Cast<IDamagable>(Actor))
	{
		if (character->IsSelectedTalent[1][0])
		{
			if (WSkillTalentQuest->GetQuestState())
			{
				KerunCharacter->ImproveEStack(WSkillTalentQuest->GetEStackAmount());
			}
			else
			{
				float hp = Target->GetHP() - damage;

				if (hp <= 0)
				{
					WSkillTalentQuest->IncreaseQuestGoal(1);
				}
			}

		}
		
		if (character->IsSelectedTalent[5][1])
		{
			FString name = "KerunWSkill Talent51";
			character->AddReduceOtherHeal(name , Kerun51ReduceHealAmount, Kerun51ReduceHealDuration);

			Target->TakeDamage(damage, Cast<AActor>(character));

			character->RemoveReduceOtherHeal(name);
		}
		else
		{
			Target->TakeDamage(damage, Cast<AActor>(character));
		}

	}

	if (character->IsSelectedTalent[1][1])
	{
		if (ICCable* Target = Cast<ICCable>(Actor))
		{
			Target->Slow(3.f, 0.5f, "KerunWSkillTalent");
		}
	}

	if (character->IsSelectedTalent[1][2])
	{
		character->AddBarrierAmount(Kerun12BarrierAmount);
	}
}

void UKerunWSkill::LandingIntoTarget(AKerunCharacter* Owner)
{
	if (AKerunCharacter* Kerun = Cast<AKerunCharacter>(Owner))
	{
		Kerun->PlayWSkillLandingSound();
	}

	//SpawnCollider();
	Owner->GetCapsuleComponent()->SetCollisionProfileName(FName("IgnoreOnlyPawn"));

	FVector StartLoc = Owner->GetActorLocation();
	
	FVector Velocity = TargetLocation - StartLoc;

	FVector TotalVelocity = Velocity + PrevVelocity * 0.5f;

	TotalVelocity *= Speed;

	Owner->GetCharacterMovement()->Launch(TotalVelocity);

	IsWorking = false;
}

