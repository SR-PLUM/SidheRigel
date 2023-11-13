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

UKerunWSkill();

public:
	virtual void SetSkillProperty(class ASidheRigelCharacter* Character, E_SkillState SkillState) override;
	virtual void OnTick() override;
	virtual void OnUse(FHitResult Hit) override;
	virtual bool CanUse() override;

private:
	FVector TargetLocation = FVector::ZeroVector;

	//Z value
	double ZValue = 300.0f;

	//Speed
	int32 Speed = 5;

	double damage = 10.0f;

	bool IsWorking = false;

private: //Talent
	float Kerun11SlowTime = 3.f;
	float Kerun11SlowAmount = -0.5f;
	float Kerun12BarrierAmount = 30.f;

	int32 Kerun51ReduceHealAmount = 30.f;
	float Kerun51ReduceHealDuration = 3.f;

	UPROPERTY()
	class UKerunWSkillTalentQuest* WSkillTalentQuest;

protected:
	void JumpIntoTarget(AActor* Actor);
	bool GetIsWorking();
	void SetIsWorking(bool flag);
	void KnockDownTarget(class AKerunCharacter* Owner);
	void AttackTarget(AActor* Actor);
	double GetLimitZValue();
};
