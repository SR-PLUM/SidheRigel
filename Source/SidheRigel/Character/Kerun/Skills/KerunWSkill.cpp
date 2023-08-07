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
		

		// Move Owner
		FVector StartLoc = Owner->GetActorLocation();
		FVector EndLoc = Actor->GetActorLocation();

		FVector OutVelocity = FVector::ZeroVector;
		FCollisionResponseParams CollisionResponseParams;


		if (UGameplayStatics::SuggestProjectileVelocity(Owner->GetWorld(),OutVelocity, StartLoc, EndLoc, 2.0f,true, 0.0f, 0.0f))
		{
			//Owner->ProjectileMovementComponent->AddForce(OutVelocity);

			//Owner->GetCharacterMovement()->AddImpulse(OutVelocity, true);

			OutVelocity *= 2;
			IsWorking = true;
			Owner->GetCharacterMovement()->Launch(OutVelocity);

			if (IDamagable* Target = Cast<IDamagable>(Actor))
			{
				Target->TakeDamage(10.0f, Cast<AActor>(Owner));
			}
		}

		UE_LOG(LogTemp, Warning, TEXT("X :: %f, Y :: %f, Z :: %f"), OutVelocity.X, OutVelocity.Y, OutVelocity.Z);

		//CoolTime
		Owner->GetWorldTimerManager().SetTimer(CoolingTimer,
			FTimerDelegate::CreateLambda([&]() {
				IsCoolingDown = false;
		GEngine->AddOnScreenDebugMessage(-1, 2, FColor::Red, FString::Printf(TEXT("Kerun R1Skill Cool Downed")));
				}), CoolDownDuration, false);
	}
	else
	{
		GEngine->AddOnScreenDebugMessage(-1, 2, FColor::Red, FString::Printf(TEXT("Kerun R1Skill Cooling Down")));
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

FVector UKerunWSkill::GetVelocity()
{
	return Velocity;
}

