#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Character.h"

#include "SidheRigel/SidheRigelCharacter.h"
#include "SidheRigel/Enum/E_SkillState.h"

#include "FairyWingCharacter.generated.h"

UCLASS()
class SIDHERIGEL_API AFairyWingCharacter : public ASidheRigelCharacter
{
	GENERATED_BODY()

public:
	AFairyWingCharacter();

protected:
	virtual void BeginPlay() override;
	virtual void Tick(float DeltaTime) override;

	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;

protected:	//Attack
	UPROPERTY(EditDefaultsOnly)
		TSubclassOf<class AFairyWingAttackProjectile> attackProjectileClass;

	virtual void InitAttackProjectile() override;
	virtual void Attack(AActor* target) override;

public:
	virtual void InitProperty() override;

private:	//State
	UPROPERTY()
		FTimerHandle destroyTimer;

protected:	//Talent
	void InitFairyWingTalent();

protected:	//Skill Projectile
	UPROPERTY(EditDefaultsOnly)
		TSubclassOf<class AFairyWingEProjectile> EProjectileClass;

	UPROPERTY(EditDefaultsOnly)
		TSubclassOf<class AFairyWingQCollider> QColliderClass;

	UPROPERTY(EditDefaultsOnly)
		TSubclassOf<class AFairyWingWCollider> WColliderClass;

	UPROPERTY(EditDefaultsOnly)
		TSubclassOf<class AFairyWingRCollider> RColliderClass;

public:
	UPROPERTY(EditDefaultsOnly, Category = Montages)
		class UAnimMontage* AttackMontage;

	UPROPERTY(EditDefaultsOnly, Category = Montages)
		class UAnimMontage* QSkillMontage;

	UPROPERTY(EditDefaultsOnly, Category = Montages)
		class UAnimMontage* WSkillMontage;

	UPROPERTY(EditDefaultsOnly, Category = Montages)
		class UAnimMontage* ESkillMontage;

	UPROPERTY(EditDefaultsOnly, Category = Montages)
		class UAnimMontage* RSkillMontage;

protected:
	void PlayAttackMontage();

public:
	void PlayQSkillMontage();
	void PlayWSkillMontage();
	void PlayESkillMontage();
	void PlayRSkillMontage();
};
