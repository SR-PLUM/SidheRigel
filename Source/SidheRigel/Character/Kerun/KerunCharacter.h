// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "SidheRigel/SidheRigelCharacter.h"
#include "../../Enum/E_SkillState.h"
#include "KerunCharacter.generated.h"

/**
 * 
 */
UCLASS()
class SIDHERIGEL_API AKerunCharacter : public ASidheRigelCharacter
{
	GENERATED_BODY()

public:
	AKerunCharacter();

protected:

	virtual void BeginPlay() override;

	virtual void Tick(float DeltaTime);

	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;

public:
	virtual void InitProperty() override;

protected:
	virtual void SpawnAttackProjectile() override;
	virtual void InitAttackProjectile() override;

	UPROPERTY(EditDefaultsOnly)
		TSubclassOf<class AKerunAttackProjectile> ProjectileClass;

	virtual void SetCurrentHP(float _hp) override;

private:

	class UKerunAnimInstance* AnimInstance;

	UPROPERTY()
	class UKerunQSkill* KerunQSkillRef;

	UPROPERTY()
	class UKerunR1Skill* KerunR1SkillRef;

	UPROPERTY()
	class UKerunR2Skill* KerunR2SkillRef;



protected:	//Skill
	virtual void SkillOne();	//Q
	//virtual void SkillTwo();	//W
	virtual void SkillFour();	//R

	virtual void SkillCancel();
	virtual void UseSkill(FHitResult HitResult);

private:	//State
	E_SkillState skillState = E_SkillState::Null;

protected: //Passive E

	const int EMaxStack = 25;
	const float EDuration = 3.5f;
	const float EHealthRate = 1.0f;

	UPROPERTY()
	int ECurrentStack = 0;

	UPROPERTY()
	FTimerHandle ETimer;

	void ImproveEStack(int Count);

	void StartETimer();
	void QuitETimer();


/*
	virtual	void SkillThree();
*/
};
