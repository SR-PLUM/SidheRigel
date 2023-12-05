// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Animation/AnimInstance.h"
#include "ColdAnimInstance.generated.h"

/**
 * 
 */
UCLASS()
class SIDHERIGEL_API UColdAnimInstance : public UAnimInstance
{
	GENERATED_BODY()
	
public:
	virtual void NativeInitializeAnimation() override;
	virtual void NativeUpdateAnimation(float DeltaTime) override;

	UPROPERTY(BlueprintReadOnly)
		class AColdCharacter* ColdCharacter;

	class UStateMachine* ColdStateMachine;

	UPROPERTY(BlueprintReadOnly)
		class UCharacterMovementComponent* ColdCharacterMovement;

	UPROPERTY(BlueprintReadOnly)
		float Speed;

	UPROPERTY(BlueprintReadOnly)
		bool isAttack;

	UPROPERTY(BlueprintREadOnly)
		bool isQUse;

	UPROPERTY(BlueprintReadOnly)
		bool isWUse;

	UPROPERTY(BlueprintReadOnly)
		bool isEUse;

	UPROPERTY(BlueprintReadOnly)
		bool isRUse;

	FTimerHandle qAnimationTimer;
	float qAnimationDuration;

	FTimerHandle wAnimationTimer;
	float wAnimationDuration;

	FTimerHandle eAnimationTimer;
	float eAnimationDuration;

	FTimerHandle rAnimationTimer;
	float rAnimationDuration;
};
