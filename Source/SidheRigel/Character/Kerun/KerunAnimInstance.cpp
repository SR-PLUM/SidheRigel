// Fill out your copyright notice in the Description page of Project Settings.

#include "KerunAnimInstance.h"
#include "KerunCharacter.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "Kismet/KismetMathLibrary.h"

void UKerunAnimInstance::NativeInitializeAnimation()
{
	KerunCharacter = Cast<AKerunCharacter>(TryGetPawnOwner());
	if (KerunCharacter) {
		KerunCharacterMovement = KerunCharacter->GetCharacterMovement();
	}
	
}

void UKerunAnimInstance::NativeUpdateAnimation(float DeltaTime)
{
	Super::NativeUpdateAnimation(DeltaTime);
	if (KerunCharacter) {
		Speed = UKismetMathLibrary::VSizeXY(KerunCharacterMovement->Velocity);
	}
	
}
