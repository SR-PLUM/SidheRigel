// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Character.h"
#include "SidheRigel/SidheRigelCharacter.h"
#include "SidheRigel/Enum/E_SkillState.h"
#include "ColdCharacter.generated.h"

UCLASS()
class SIDHERIGEL_API AColdCharacter : public ASidheRigelCharacter
{
	GENERATED_BODY()

public:
	// Sets default values for this character's properties
	AColdCharacter();

protected:
	virtual void BeginPlay() override;
	virtual void Tick(float DeltaTime) override;

	// Called to bind functionality to input
	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;

protected:	//Attack
	UPROPERTY(EditDefaultsOnly)
		TSubclassOf<class AColdAttackProjectile> attackProjectileClass;
	
	virtual void InitAttackProjectile() override;
	virtual void SpawnAttackProjectile() override;

protected:	//Skill
	virtual void SkillOne();
	void QImplement(FHitResult HitResult);

	virtual void SkillTwo();
	void WImplement(FHitResult HitResult);

	virtual	void SkillThree();

	virtual void SkillFour();

	virtual void SkillCancel();
	virtual void UseSkill(FHitResult HitResult);

private:	//State
	E_SkillState skillState;

protected:	//Skill Projectile
	UPROPERTY(EditDefaultsOnly)
		TSubclassOf<class AColdQProjectile> QProjectileClass;
	UPROPERTY(EditDefaultsOnly)
		TSubclassOf<class AColdWProjectile> WProjectileClass;

private:	//Skill Variable
	int32 QCount = 10;
};
