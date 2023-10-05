// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Character.h"

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

private:	//State
	E_UltType ultType;


protected:	//Skill Projectile
	UPROPERTY(EditDefaultsOnly)
		TSubclassOf<class AColdQProjectile> QProjectileClass;
	UPROPERTY(EditDefaultsOnly)
		TSubclassOf<class AColdWProjectile> WProjectileClass;
	UPROPERTY(EditDefaultsOnly)
		TSubclassOf<class AColdEProjectile> EProjectileClass;
	UPROPERTY(EditDefaultsOnly)
		TSubclassOf<class AColdEDamageField>EDamageFieldClass;
	UPROPERTY(EditDefaultsOnly)
		TSubclassOf<class AColdR1Projectile>R1ProjectileClass;
	UPROPERTY(EditDefaultsOnly)
		TSubclassOf<class AColdR2Projectile>R2ProjectileClass;

public:

private:	//Skill Variable
	int32 QCount = 10;
};
