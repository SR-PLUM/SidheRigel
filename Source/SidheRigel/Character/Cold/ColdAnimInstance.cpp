// Fill out your copyright notice in the Description page of Project Settings.


#include "ColdAnimInstance.h"
#include "ColdCharacter.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "Kismet/KismetMathLibrary.h"

#include "SidheRigel/SidheRigelPlayerController.h"

void UColdAnimInstance::NativeInitializeAnimation()
{
	Super::NativeInitializeAnimation();

	ColdCharacter = Cast<AColdCharacter>(TryGetPawnOwner());
	if (ColdCharacter)
	{
		ColdCharacterMovement = ColdCharacter->GetCharacterMovement();

		if (auto SRController = Cast<ASidheRigelPlayerController>(ColdCharacter->GetController()))
		{
			ColdStateMachine = SRController->stateMachine;
		}
	}
}

void UColdAnimInstance::NativeUpdateAnimation(float DeltaTime)
{
	Super::NativeUpdateAnimation(DeltaTime);

	if (ColdCharacterMovement)
	{
		Speed = UKismetMathLibrary::VSizeXY(ColdCharacterMovement->Velocity);
		if (ColdStateMachine)
		{
			if (ColdStateMachine->GetCurrentState() == ColdStateMachine->Attack)
			{
				isAttack = true;
			}
			else
			{
				isAttack = false;
			}
		}
		else
		{
			if (auto SRController = Cast<ASidheRigelPlayerController>(ColdCharacter->GetController()))
			{
				ColdStateMachine = SRController->stateMachine;
			}
		}
	}
}