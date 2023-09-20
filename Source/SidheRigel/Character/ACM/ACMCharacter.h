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

private :
	int32 QCoolDown = 5;
	int32 WCoolDown = 12;
	int32 ECoolDown = 4;
	int32 R1CoolDown = 20;
	int32 R2CoolDown = 65;
	float ReduceCoolDownAmount = 0.8f;

public:
	// Sets default values for this character's properties
	AACMCharacter();

protected:

	virtual void BeginPlay() override;
	virtual void Tick(float DeltaTime) override;

	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;

public:
	virtual void InitProperty() override;

	void InitColliderPath();
	
protected:
	virtual void Attack(AActor* target) override;
	virtual void InitAttackProjectile() override;

	//AttackProjectile
	UPROPERTY(EditDefaultsOnly)
	TSubclassOf<class AACMAttackProjectile> ProjectileClass;

protected: //Skills
	void QSkill(FHitResult HitResult);
	void QImplement(FHitResult HitResult);
	void ESkill(FHitResult HitResult);
	void EImplement(FHitResult HitResult);
	void RSkill(FHitResult HitResult);
	void R1Skill(FHitResult HitResult);


	virtual void UseSkill(FHitResult HitResult, E_SkillState SkillState);


private:
	E_SkillState LastSkill = E_SkillState::Null;

	//Upgrading Passive
	int32 SkillCount= 0;
	bool UpgradeNextSkill = false;
	float CountDuration = 8.f;
	FTimerHandle UpgradingPassiveTimer;

	//CoolDown
	float QCoolDownRemain = 0.0f;
	float WCoolDownRemain = 0.0f;
	float ECoolDownRemain = 0.0f;
	float R1CoolDownRemain = 0.0f;
	float R2CoolDownRemain = 0.0f;

protected:
	void AddSkillCount();
	void InitializeSkillCount();

	void ReduceCoolDownRemain();

protected: //Skill Collider
	UPROPERTY(EditDefaultsOnly)
	TSubclassOf<class AACMQCollider> ACMQCollider;

	UPROPERTY(EditDefaultsOnly)
	TSubclassOf<class AACMECollider> ACMECollider;
};
