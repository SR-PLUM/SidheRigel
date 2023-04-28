// Fill out your copyright notice in the Description page of Project Settings.


#include "BlackWizardAnimInstance.h"
#include "BlackWizardCharacter.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "Kismet/KismetMathLibrary.h"

void UBlackWizardAnimInstance::NativeInitializeAnimation()
{
	Super::NativeInitializeAnimation();

	BlackWizardCharacter = Cast<ABlackWizardCharacter>(TryGetPawnOwner());
	if (BlackWizardCharacter)
	{
		BlackWizardCharacterMovement = BlackWizardCharacter->GetCharacterMovement();
	}
}

void UBlackWizardAnimInstance::NativeUpdateAnimation(float DeltaTime)
{
	Super::NativeUpdateAnimation(DeltaTime);

	if (BlackWizardCharacterMovement)
	{
		Speed = UKismetMathLibrary::VSizeXY(BlackWizardCharacterMovement->Velocity);
	}
}
