#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Character.h"
#include "SidheRigel/SidheRigelCharacter.h"
#include "SidheRigel/Enum/E_SkillState.h"
#include "BlackWizardCharacter.generated.h"

UCLASS()
class SIDHERIGEL_API ABlackWizardCharacter : public ASidheRigelCharacter
{
	GENERATED_BODY()

public:
	// Sets default values for this character's properties
	ABlackWizardCharacter();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;
	virtual void Tick(float DeltaTime) override;
	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;

protected:	//Attack
	UPROPERTY(EditDefaultsOnly)
		TSubclassOf<class ABlackWizardAttackProjectile> attackProjectileClass;

	virtual void InitAttackProjectile() override;
	virtual void Attack(AActor* target) override;

private:
	UPROPERTY()
		FTimerHandle destroyTimer;

protected:	//Skill Projectile
	UPROPERTY(EditDefaultsOnly)
		TSubclassOf<class ABlackWizardQCollider> QColliderClass;
	UPROPERTY(EditDefaultsOnly)
		TSubclassOf<class ABlackWizardECollider> EColliderClass;
	UPROPERTY(EditDefaultsOnly)
		TSubclassOf<class ABlackWizardRCollider> RColliderClass;
};
