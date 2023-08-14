// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "SidheRigel/SidheRigelCharacter.h"
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
	
protected:
	virtual void SpawnAttackProjectile() override;
	virtual void InitAttackProjectile() override;

	UPROPERTY(EditDefaultsOnly)
	TSubclassOf<class AACMAttackProjectile> ProjectileClass;

public:
	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite)
	class UProjectileMovementComponent* ProjectileMovementComponent;
};
