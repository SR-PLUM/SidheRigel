// Fill out your copyright notice in the Description page of Project Settings.


#include "FairyWingAnimInstance.h"
#include "FairyWingCharacter.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "Kismet/KismetMathLibrary.h"

void UFairyWingAnimInstance::NativeInitializeAnimation()
{
	Super::NativeInitializeAnimation();

	FairyWingCharacter = Cast<AFairyWingCharacter>(TryGetPawnOwner());
	if (FairyWingCharacter)
	{
		FairyWingCharacterMovement = FairyWingCharacter->GetCharacterMovement();
	}
}

void UFairyWingAnimInstance::NativeUpdateAnimation(float DeltaTime)
{
	Super::NativeUpdateAnimation(DeltaTime);

	if (FairyWingCharacterMovement)
	{
		Speed = UKismetMathLibrary::VSizeXY(FairyWingCharacterMovement->Velocity);
	}
}

