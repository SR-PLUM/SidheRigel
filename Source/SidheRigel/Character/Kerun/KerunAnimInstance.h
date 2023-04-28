// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Animation/AnimInstance.h"
#include "KerunAnimInstance.generated.h"

/**
 * 
 */
UCLASS()
class SIDHERIGEL_API UKerunAnimInstance : public UAnimInstance
{
	GENERATED_BODY()

public:
	virtual void NativeInitializeAnimation() override;

	virtual void NativeUpdateAnimation(float DeltaTime) override;

protected:
	UPROPERTY(BlueprintReadOnly)
	class AKerunCharacter* KerunCharacter;

	UPROPERTY(BlueprintReadOnly)
	class UCharacterMovementComponent* KerunCharacterMovement;

	UPROPERTY(BlueprintReadOnly)
	float Speed;
	
};
