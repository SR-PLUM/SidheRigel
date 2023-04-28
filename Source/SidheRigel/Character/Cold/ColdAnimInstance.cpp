// Fill out your copyright notice in the Description page of Project Settings.


#include "ColdAnimInstance.h"
#include "ColdCharacter.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "Kismet/KismetMathLibrary.h"

void UColdAnimInstance::NativeInitializeAnimation()
{
	Super::NativeInitializeAnimation();

	ColdCharacter = Cast<AColdCharacter>(TryGetPawnOwner());
	if (ColdCharacter)
	{
		ColdCharacterMovement = ColdCharacter->GetCharacterMovement();
	}
}

void UColdAnimInstance::NativeUpdateAnimation(float DeltaTime)
{
	Super::NativeUpdateAnimation(DeltaTime);

	if (ColdCharacterMovement)
	{
		Speed = UKismetMathLibrary::VSizeXY(ColdCharacterMovement->Velocity);
	}
}