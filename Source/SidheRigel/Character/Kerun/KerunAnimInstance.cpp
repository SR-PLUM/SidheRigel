// Fill out your copyright notice in the Description page of Project Settings.

#include "KerunAnimInstance.h"
#include "KerunCharacter.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "Kismet/KismetMathLibrary.h"

UKerunAnimInstance::UKerunAnimInstance()
{
	InitAttackAnimation();
}

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

void UKerunAnimInstance::PlayAttackMontage()
{
	Montage_Play(AttackAnim, 1.0f);
}

void UKerunAnimInstance::InitAttackAnimation()
{
	static ConstructorHelpers::FObjectFinder<UAnimMontage>AttackAnimMontage(TEXT("/Game/Heros/Kerun/Animations/Melee_A_Montage"));
	if (AttackAnimMontage.Succeeded())
	{
		AttackAnim = AttackAnimMontage.Object;
	}
}
