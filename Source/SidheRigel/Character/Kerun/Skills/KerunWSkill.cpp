// Fill out your copyright notice in the Description page of Project Settings.


#include "KerunWSkill.h"
#include "../KerunCharacter.h"
#include "SidheRigel/Interface/Damagable.h"
#include "GameFramework/ProjectileMovementComponent.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "Kismet/GameplayStatics.h"
#include "Kismet/GameplayStaticsTypes.h"

void UKerunWSkill::SetSkillProperty(ASidheRigelCharacter* Character, E_SkillState SkillState)
{
	skillDelay = 1.f;
	skillCooldown = 0.f;
	skillMaxCooldown = 10.f;
	range = 300.f;

	bIsInstantCast = false;
	bIsTargeting = true;

	character = Character;
	skillstate = SkillState;

	character->GetWorldTimerManager().SetTimer(cooldownTimer, this, &UKerunWSkill::OnTick, 0.1f, true);
}

void UKerunWSkill::OnTick()
{
	Super::OnTick();

	//Kerun WSkill
	if (GetIsWorking())
	{
		FVector Loc = character->GetActorLocation();

		if (Loc.Z >= GetLimitZValue())
		{
			AKerunCharacter* KerunCharacter = Cast<AKerunCharacter>(character);
			KnockDownTarget(KerunCharacter);
		}

	}
}

void UKerunWSkill::OnUse(FHitResult Hit)
{
	if (AActor* Actor = Hit.GetActor())
	{
		if (IDamagable* Target = Cast<IDamagable>(Actor))
		{
			JumpIntoTarget(Actor);

			AKerunCharacter* KerunCharacter = Cast<AKerunCharacter>(character);

			KerunCharacter->ImproveEStack(3);
		}
	}
}

void UKerunWSkill::JumpIntoTarget(AActor* Actor)
{
	IsWorking = true;
	// Move Owner
	FVector StartLoc = character->GetActorLocation();
	FVector EndLoc = Actor->GetActorLocation();
	TargetLocation = EndLoc;
	FVector Velocity = EndLoc - StartLoc;
	Velocity.Z = StartLoc.Z + ZValue;
	Velocity *= Speed - 1;

	character->GetCharacterMovement()->Launch(Velocity);
	if (IDamagable* Target = Cast<IDamagable>(Actor))
	{
		Target->TakeDamage(10.0f, Cast<AActor>(character));
	}

	if (character->IsSelectedTalent[1][1])
	{
		if (ICCable* Target = Cast<ICCable>(Actor))
		{
			Target->Slow(3.f, -0.5f, "KerunWSkillTalent");
		}
	}

	if (character->IsSelectedTalent[1][2])
	{
		character->AddBarrierAmount(Kerun12BarrierAmount);
	}
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
	FVector StartLoc = Owner->GetActorLocation();
	
	FVector Velocity = TargetLocation - StartLoc;

	Velocity *= Speed;

	Owner->GetCharacterMovement()->Launch(Velocity);

	IsWorking = false;
}

double UKerunWSkill::GetLimitZValue()
{
	return TargetLocation.Z + ZValue;
}