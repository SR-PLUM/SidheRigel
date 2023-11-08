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
	virtual void Attack(AActor* target) override;

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
};
