// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "SidheRigel/SidheRigelCharacter.h"
#include "SidheRigel/Enum/E_SkillState.h"
#include "ACMCharacter.generated.h"

/**
 * 
 */
UCLASS()
class SIDHERIGEL_API AACMCharacter : public ASidheRigelCharacter
{
	GENERATED_BODY()

public:
	// Sets default values for this character's properties
	AACMCharacter();

protected:

	virtual void BeginPlay() override;

	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;

public:
	virtual void InitProperty() override;

	void InitColliderPath();
	
protected:
	virtual void SpawnAttackProjectile() override;
	virtual void InitAttackProjectile() override;

	//AttackProjectile
	UPROPERTY(EditDefaultsOnly)
	TSubclassOf<class AACMAttackProjectile> ProjectileClass;

protected: //Skills
	virtual void SkillOne();
	void QSkill(FHitResult HitResult);
	virtual void SkillThree();
	void ESkill(FHitResult HitResult);


	virtual void SkillCancel();
	virtual void UseSkill(FHitResult HitResult);

private:
	E_SkillState skillState = E_SkillState::Null;

	bool UpgradeNextSkill= false;

protected: //Skill Collider
	UPROPERTY(EditDefaultsOnly)
	TSubclassOf<class AACMQCollider> ACMQCollider;

	UPROPERTY(EditDefaultsOnly)
	TSubclassOf<class AACMECollider> ACMECollider;
};
