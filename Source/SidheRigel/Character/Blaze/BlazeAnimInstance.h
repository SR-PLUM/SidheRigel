// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Animation/AnimInstance.h"
#include "BlazeAnimInstance.generated.h"

/**
 * 
 */
UCLASS()
class SIDHERIGEL_API UBlazeAnimInstance : public UAnimInstance
{
	GENERATED_BODY()
	
public:
	virtual void NativeInitializeAnimation() override;
	virtual void NativeUpdateAnimation(float DeltaTime) override;
	
	UPROPERTY(BlueprintReadOnly)
	class ABlazeCharacter* BlazeCharacter;

	UPROPERTY(BlueprintReadOnly)
	class UCharacterMovementComponent* BlazeCharacterMovement;

	UPROPERTY(BlueprintReadOnly)
	float Speed;
};
