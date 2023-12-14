// Fill out your copyright notice in the Description page of Project Settings.


#include "ACMAnimInstance.h"
#include "ACMCharacter.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "Kismet/KismetMathLibrary.h"

void UACMAnimInstance::NativeInitializeAnimation()
{
	Super::NativeInitializeAnimation();

	ACMCharacter = Cast<AACMCharacter>(TryGetPawnOwner());

	if (ACMCharacter) 
	{
		ACMCharacterMovement = ACMCharacter->GetCharacterMovement();
	}
}

void UACMAnimInstance::NativeUpdateAnimation(float DeltaTime)
{
	Super::NativeUpdateAnimation(DeltaTime);
	
	if (ACMCharacterMovement) 
	{
		Speed = UKismetMathLibrary::VSizeXY(ACMCharacterMovement->Velocity);
	}
	
}
