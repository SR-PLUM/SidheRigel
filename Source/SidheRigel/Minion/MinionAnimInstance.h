// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Animation/AnimInstance.h"
#include "MinionAnimInstance.generated.h"

/**
 * 
 */
UCLASS()
class SIDHERIGEL_API UMinionAnimInstance : public UAnimInstance
{
	GENERATED_BODY()
	
public:
	virtual void NativeInitializeAnimation() override;
	virtual void NativeUpdateAnimation(float DeltaTime) override;

	UPROPERTY(BlueprintReadOnly)
		class AMinion* Minion;

	UPROPERTY(BlueprintReadOnly)
		class UCharacterMovementComponent* MinionMovement;

	UPROPERTY(BlueprintReadOnly)
		float Speed;

	UPROPERTY(BlueprintReadOnly)
		bool IsAttackAnim;
};
