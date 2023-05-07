// Copyright Epic Games, Inc. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Character.h"
#include "Interface/Attackable.h"
#include "Interface/CCable.h"
#include "Interface/Damagable.h"
#include "Interface/Movable.h"
#include "SidheRigelCharacter.generated.h"

UCLASS(Blueprintable)
class ASidheRigelCharacter : public ACharacter, public IAttackable, public ICCable, public IDamagable, public IMovable
{
	GENERATED_BODY()

public:
	ASidheRigelCharacter();

	virtual void BeginPlay();
	virtual void Tick(float DeltaSeconds) override;

	void SetupPlayerInputComponent(UInputComponent* PlayerInputComponent);

	/** Returns TopDownCameraComponent subobject **/
	FORCEINLINE class UCameraComponent* GetTopDownCameraComponent() const { return TopDownCameraComponent; }
	/** Returns CameraBoom subobject **/
	FORCEINLINE class USpringArmComponent* GetCameraBoom() const { return CameraBoom; }

protected:
	/** Top down camera */
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = Camera, meta = (AllowPrivateAccess = "true"))
		class UCameraComponent* TopDownCameraComponent;

	/** Camera boom positioning the camera above the character */
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = Camera, meta = (AllowPrivateAccess = "true"))
		class USpringArmComponent* CameraBoom;

	// Projectile class to spawn.
	UPROPERTY(EditDefaultsOnly)
		TSubclassOf<class ADummyProjectile> ProjectileClass;

protected:	//Skill
	UFUNCTION()
		virtual void SkillOne();
	UFUNCTION()
		virtual void SkillTwo();
	UFUNCTION()
		virtual void SkillThree();
	UFUNCTION()
		virtual void SkillFour();
	UFUNCTION()
		virtual void SkillCancel();
	UFUNCTION()
		virtual void UseSkill(AActor* target);

protected:	//Stat
	UPROPERTY()
		int32 level;							//레벨
	UPROPERTY()
		int32 experience;						//현재 보유하고 있는 경험치
	UPROPERTY()
		float currentHP;						//현재 체력
	UPROPERTY()
		TMap<FString, float> range;				//레벨업을 제외한 다른 요인들에 의해 증가되는 사거리 딕셔너리
	UPROPERTY()
		TMap<FString, float> speed;				//레벨업을 제외한 다른 요인들에 의해 증가되는 이동속도 딕셔너리
	UPROPERTY()
		TMap<FString, float> attackDamage;		//레벨업을 제외한 다른 요인들에 의해 증가되는 공격력 딕셔너리
	UPROPERTY()
		TMap<FString, float> abiltyPower;		//레벨업을 제외한 다른 요인들에 의해 증가되는 주문력 딕셔너리
	UPROPERTY()
		TMap<FString, int32> criticalRate;		//레벨업을 제외한 다른 요인들에 의해 증가되는 치명타 확률 딕셔너리
	UPROPERTY()
		TMap<FString, int32> criticalDamage;	//레벨업을 제외한 다른 요인들에 의해 증가되는 치명타 피해 딕셔너리
	UPROPERTY()
		TMap<FString, float> attackSpeed;		//레벨업을 제외한 다른 요인들에 의해 증가되는 치명타 피해 딕셔너리
	UPROPERTY()
		TMap<FString, float> MaxHP;				//레벨업을 제외한 다른 요인들에 의해 증가되는 체력 딕셔너리
	UPROPERTY()
		TMap<FString, float> generateHealthPoint;	//레벨업을 제외한 다른 요인들에 의해 증가되는 체력 재생량 딕셔너리
	UPROPERTY()
		TMap<FString, float> manaPoint;			//레벨업을 제외한 다른 요인들에 의해 증가되는 마나 딕셔너리
	UPROPERTY()
		TMap<FString, float> defencePoint;		//레벨업을 제외한 다른 요인들에 의해 증가되는 방어력 딕셔너리
	UPROPERTY()
		TMap<FString, float> magicResistPoint;	//레벨업을 제외한 다른 요인들에 의해 증가되는 마법저항력 딕셔너리
	UPROPERTY()
		TMap<FString, float> armorPenetration;	//레벨업을 제외한 다른 요인들에 의해 증가되는 물리관통력 딕셔너리
	UPROPERTY()
		TMap<FString, float> magicPenetration;	//레벨업을 제외한 다른 요인들에 의해 증가되는 마법관통력 딕셔너리
	UPROPERTY()
		TMap<FString, int32> cooldownReduction;	//레벨업을 제외한 다른 요인들에 의해 증가되는 쿨타임감소 딕셔너리
	UPROPERTY()
		TMap<FString, float> lifeSteal;			//레벨업을 제외한 다른 요인들에 의해 증가되는 흡혈 딕셔너리
	UPROPERTY()
		TMap<FString, float> protectPower;		//레벨업을 제외한 다른 요인들에 의해 증가되는 회복 및 보호막 효과 딕셔너리
	UPROPERTY()
		TMap<FString, float> endurance;			//레벨업을 제외한 다른 요인들에 의해 증가되는 인내심 딕셔너리

public:		//Getter, Setter
	void SetLevel(int32 _level);
	void SetTarget(AActor* target);
	float GetRange();
	float GetAttackSpeed();

protected:	//Attack
	bool bAttackDelay;
	void SetAttackDelayFalse();

	AActor* target;

public:		//Interface Implement
	UFUNCTION()
		virtual void Attack(AActor* Target) override;

	UFUNCTION()
		virtual void Stun(float time) override;
	UFUNCTION()
		virtual void Stop(float time) override;
	UFUNCTION()
		virtual void Slow(float time, float value) override;
	UFUNCTION()
		virtual void Silence(float time) override;
	UFUNCTION()
		virtual void Airborne(float time) override;

	UFUNCTION()
		virtual void TakeDamage(float damage) override;
	UFUNCTION()
		virtual void RestoreHP(float value) override;
};

