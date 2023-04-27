// Fill out your copyright notice in the Description page of Project Settings.


#include "BlazeAnimInstance.h"
#include "BlazeCharacter.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "Kismet/KismetMathLibrary.h"

void UBlazeAnimInstance::NativeInitializeAnimation()
{
	Super::NativeInitializeAnimation();

	BlazeCharacter = Cast<ABlazeCharacter>(TryGetPawnOwner());
	if (BlazeCharacter)
	{
		BlazeCharacterMovement = BlazeCharacter->GetCharacterMovement();
	}
}

void UBlazeAnimInstance::NativeUpdateAnimation(float DeltaTime)
{
	Super::NativeUpdateAnimation(DeltaTime);

	if (BlazeCharacterMovement)
	{
		Speed = UKismetMathLibrary::VSizeXY(BlazeCharacterMovement->Velocity);
	}
}
