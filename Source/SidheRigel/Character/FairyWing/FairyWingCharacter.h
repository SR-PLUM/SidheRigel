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
	// Sets default values for this character's properties
	AFairyWingCharacter();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;
	virtual void Tick(float DeltaTime) override;

	// Called to bind functionality to input
	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;

protected:	//Attack
	UPROPERTY(EditDefaultsOnly)
		TSubclassOf<class AFairyWingAttackProjectile> attackProjectileClass;

	virtual void InitAttackProjectile() override;
	virtual void SpawnAttackProjectile() override;

protected:	//Skill
	virtual void SkillOne();
	virtual void SkillTwo();
	virtual	void SkillThree();
	virtual void SkillFour();

	virtual void SkillCancel();
	virtual void UseSkill(FHitResult HitResult);

private:	//State
	E_SkillState skillState; 
	UPROPERTY()
		FTimerHandle destroyTimer;

protected:	//Skill Projectile
	UPROPERTY(EditDefaultsOnly)
		TSubclassOf<class AFairyWingEProjectile> EProjectileClass;

	UPROPERTY(EditDefaultsOnly)
		TSubclassOf<class AFairyWingQCollider> QColliderClass;
};
