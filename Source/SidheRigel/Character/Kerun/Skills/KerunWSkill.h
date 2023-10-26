// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "../../Skill.h"
#include "KerunWSkill.generated.h"
/**
 * 
 */
UCLASS()
class SIDHERIGEL_API UKerunWSkill : public USkill
{
	GENERATED_BODY()

public:
	virtual void SetSkillProperty(class ASidheRigelCharacter* Character, E_SkillState SkillState) override;
	virtual void OnTick(float DeltaTime) override;
	virtual void OnUse(FHitResult Hit) override;

private:
	FVector TargetLocation = FVector::ZeroVector;

	//Z value
	double ZValue = 300.0f;

	//Speed
	int32 Speed = 5;

	bool IsWorking = false;

protected:
	void JumpIntoTarget(AActor* Actor);
	bool GetIsWorking();
	void SetIsWorking(bool flag);
	void KnockDownTarget(class AKerunCharacter* Owner);
	double GetLimitZValue();
};
