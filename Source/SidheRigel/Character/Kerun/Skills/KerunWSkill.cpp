// Fill out your copyright notice in the Description page of Project Settings.


#include "KerunWSkill.h"
#include "../KerunCharacter.h"
#include "SidheRigel/Interface/Damagable.h"
#include "GameFramework/ProjectileMovementComponent.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "Kismet/GameplayStatics.h"
#include "Kismet/GameplayStaticsTypes.h"

void UKerunWSkill::JumpIntoTarget(AActor* Actor, AKerunCharacter* Owner)
{
	if (!(IsCoolingDown))
	{
		IsCoolingDown = true;
		IsWorking = true;
		
		// Move Owner
		FVector StartLoc = Owner->GetActorLocation();
		FVector EndLoc = Actor->GetActorLocation();

		TargetLocation = EndLoc;

		FVector Velocity = EndLoc - StartLoc;
		Velocity.Z = StartLoc.Z + ZValue;

		Velocity *= Speed - 1;
		
		Owner->GetCharacterMovement()->Launch(Velocity);

		if (IDamagable* Target = Cast<IDamagable>(Actor))
		{
			Target->TakeDamage(10.0f, Cast<AActor>(Owner));
		}

		UE_LOG(LogTemp, Warning, TEXT("X :: %f, Y :: %f, Z :: %f"), Velocity.X, Velocity.Y, Velocity.Z);

		//CoolTime
		Owner->GetWorldTimerManager().SetTimer(CoolingTimer,
			FTimerDelegate::CreateLambda([&]() {
				IsCoolingDown = false;
		GEngine->AddOnScreenDebugMessage(-1, 2, FColor::Red, FString::Printf(TEXT("Kerun WSkill Cool Downed")));
				}), CoolDownDuration, false);
	}
	else
	{
		GEngine->AddOnScreenDebugMessage(-1, 2, FColor::Red, FString::Printf(TEXT("Kerun WSkill Cooling Down")));
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