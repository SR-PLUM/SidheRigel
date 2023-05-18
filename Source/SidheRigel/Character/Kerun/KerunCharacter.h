// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "SidheRigel/SidheRigelCharacter.h"
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

protected:	//Skill
	virtual void SkillOne();

/*
	virtual void SkillTwo();
	virtual	void SkillThree();
	virtual void SkillFour();

	virtual void SkillCancel();
	virtual void UseSkill(AActor* target);

private:	//State
	E_SkillState skillState;

*/
private:
	UPROPERTY()
	class UKerunQSkill* KerunQSkillRef;
};
