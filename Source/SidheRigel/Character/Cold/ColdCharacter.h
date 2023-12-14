// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Character.h"
#include "kismet/GameplayStatics.h"

#include "SidheRigel/SidheRigelCharacter.h"
#include "SidheRigel/Enum/E_SkillState.h"
#include "SidheRigel/Enum/E_UltType.h"

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

public:
	UPROPERTY(VisibleAnywhere)
		TArray<class UStaticMeshComponent*> QMuzzle;

protected:	//Attack
	UPROPERTY(EditDefaultsOnly)
		TSubclassOf<class AColdAttackProjectile> attackProjectileClass;
	
	virtual void InitAttackProjectile() override;
	virtual void Attack(AActor* target) override;

	virtual void InitProperty() override;
	virtual void LevelUp() override;

private:	//State
	E_UltType ultType;

protected:	//Talent
	void InitColdTalent();

private:	//Skill Variable
	int32 QCount = 10;

public:
	UPROPERTY(EditDefaultsOnly, Category = Sounds)
		class USoundBase* QSkillSound;

	UPROPERTY(EditDefaultsOnly, Category = Sounds)
		class USoundBase* WSkillSound;

	UPROPERTY(EditDefaultsOnly, Category = Sounds)
		class USoundBase* ESkillSound;

	UPROPERTY(EditDefaultsOnly, Category = Sounds)
		class USoundBase* RSkillSound;

public:
	void PlayQSkillSound();
	void PlayWSkillSound();
	void PlayESkillSound();
	void PlayRSkillSound();
};
