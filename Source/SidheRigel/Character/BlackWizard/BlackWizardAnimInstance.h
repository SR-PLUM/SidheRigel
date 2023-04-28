// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Animation/AnimInstance.h"
#include "BlackWizardAnimInstance.generated.h"

/**
 * 
 */
UCLASS()
class SIDHERIGEL_API UBlackWizardAnimInstance : public UAnimInstance
{
	GENERATED_BODY()
public:
	virtual void NativeInitializeAnimation() override;
	virtual void NativeUpdateAnimation(float DeltaTime) override;

	UPROPERTY(BlueprintReadOnly)
		class ABlackWizardCharacter* BlackWizardCharacter;

	UPROPERTY(BlueprintReadOnly)
		class UCharacterMovementComponent* BlackWizardCharacterMovement;

	UPROPERTY(BlueprintReadOnly)
		float Speed;
	
};
