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

	class StateMachine* ColdStateMachine;

	UPROPERTY(BlueprintReadOnly)
		class UCharacterMovementComponent* ColdCharacterMovement;

	UPROPERTY(BlueprintReadOnly)
		float Speed;

	UPROPERTY(BlueprintReadOnly)
		bool isAttack;
};
