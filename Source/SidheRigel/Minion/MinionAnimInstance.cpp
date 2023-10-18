// Fill out your copyright notice in the Description page of Project Settings.


#include "MinionAnimInstance.h"

#include "Kismet/KismetMathLibrary.h"
#include "GameFramework/CharacterMovementComponent.h"

#include "SidheRigel/Minion/Minion.h"

void UMinionAnimInstance::NativeInitializeAnimation()
{
	Super::NativeInitializeAnimation();

	Minion = Cast<AMinion>(TryGetPawnOwner());
	if (Minion)
	{
		MinionMovement = Minion->GetCharacterMovement();
	}
}

void UMinionAnimInstance::NativeUpdateAnimation(float DeltaTime)
{
	Super::NativeUpdateAnimation(DeltaTime);

	if (MinionMovement)
	{
		Speed = UKismetMathLibrary::VSizeXY(MinionMovement->Velocity);
	}
	if (Minion)
	{
		IsAttackAnim = Minion->IsAttackAnim;
	}
}
