// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Animation/AnimInstance.h"
#include "ACMAnimInstance.generated.h"

/**
 * 
 */
UCLASS()
class SIDHERIGEL_API UACMAnimInstance : public UAnimInstance
{
	GENERATED_BODY()

public:

	virtual void NativeInitializeAnimation() override;

	virtual void NativeUpdateAnimation(float DeltaTime) override;

protected:

	UPROPERTY(BlueprintReadOnly)
	class AACMCharacter* ACMCharacter;

	UPROPERTY(BlueprintReadOnly)
	class UCharacterMovementComponent* ACMCharacterMovement;

	UPROPERTY(BlueprintReadOnly)
	float Speed;
};
