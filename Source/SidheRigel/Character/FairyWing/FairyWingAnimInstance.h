// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Animation/AnimInstance.h"
#include "FairyWingAnimInstance.generated.h"

/**
 * 
 */
UCLASS()
class SIDHERIGEL_API UFairyWingAnimInstance : public UAnimInstance
{
	GENERATED_BODY()
	
public:
	virtual void NativeInitializeAnimation() override;
	virtual void NativeUpdateAnimation(float DeltaTime) override;

	UPROPERTY(BlueprintReadOnly)
		class AFairyWingCharacter* FairyWingCharacter;

	UPROPERTY(BlueprintReadOnly)
		class UCharacterMovementComponent* FairyWingCharacterMovement;

	UPROPERTY(BlueprintReadOnly)
		float Speed;
};
