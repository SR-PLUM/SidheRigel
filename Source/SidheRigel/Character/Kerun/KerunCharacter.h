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
	virtual void Attack(AActor* target) override;
	virtual void InitAttackProjectile() override;

	UPROPERTY(EditDefaultsOnly)
		TSubclassOf<class AKerunAttackProjectile> ProjectileClass;

	virtual void SetCurrentHP(float _hp) override;

private:

	class UKerunAnimInstance* AnimInstance;

public:
	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite)
	class UProjectileMovementComponent* ProjectileMovementComponent;

protected: //Passive E

	int EMaxStack = 25;
	int EAddAmount = 1;
	const float EDuration = 3.5f;
	const float UpgradeEDuration = 5.0f;
	const float EHealthRate = 1.0f;
	const float UpgradeEHealthRate = 3.0f;

	UPROPERTY()
	int ECurrentStack = 0;

	UPROPERTY()
	FTimerHandle ETimer;

	UPROPERTY()
	class UKerunESkillTalentQuest* ESkillTalentQuest;

	float GetEDuration();
	float GetEHealthRate();
	void StartETimer();
	void QuitETimer();

public:
	void ImproveEStack(int Count);
	void SetEMaxStack(int32 Value);
	void SetEAddAmount(int32 Value);

protected:	//Talent
	int32 KerunTalent40EnduranceAmount = 20;

	void InitKerunTalent();
	
public: 
	float KerunTalent40LifeStealAmount = 25.f;
};
