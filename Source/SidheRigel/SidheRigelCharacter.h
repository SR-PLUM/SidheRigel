﻿// Copyright Epic Games, Inc. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Character.h"

#include "Interface/Attackable.h"
#include "Interface/CCable.h"
#include "Interface/Damagable.h"
#include "Interface/Movable.h"
#include "Interface/Team.h"
#include "Enum/E_SkillState.h"
#include "Character/Skill.h"

#include "SidheRigelCharacter.generated.h"

UCLASS(Blueprintable)
class ASidheRigelCharacter : public ACharacter, public IAttackable, public ICCable, public IDamagable, public IMovable, public ITeam
{
	GENERATED_BODY()

	friend class ASidheRigelPlayerController;

public:
	ASidheRigelCharacter();

	virtual void BeginPlay();
	virtual void Tick(float DeltaTime) override;

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
		TSubclassOf<class ADummyProjectile> baseProjectileClass;

public:	//Skill
	TMap<E_SkillState, Skill*> skills;
	virtual void UseSkill(FHitResult HitResult, E_SkillState SkillState);

public: //UI
	UPROPERTY()
		class UInGameUI* InGameUI;

	void SetUISkillCoolDown(E_SkillState SkillState, float Percentage, float CurrentCoolDown);
	void ClearUISkillCoolDown(E_SkillState SkillState);

protected:	//Stat
	UPROPERTY()
		int32 level;							//레벨
	UPROPERTY()
		int32 experience;						//현재 보유하고 있는 경험치
	UPROPERTY()
		int32 MaxExperience;					//최대 경험치
	UPROPERTY()
		float currentHP;						//현재 체력
	UPROPERTY()
		float currentMP;
	UPROPERTY()
		int32 money;
	UPROPERTY()
		TMap<FString, float> range;				//레벨업을 제외한 다른 요인들에 의해 증가되는 사거리 딕셔너리
	UPROPERTY()
		TMap<FString, float> speed;				//레벨업을 제외한 다른 요인들에 의해 증가되는 이동속도 딕셔너리
	UPROPERTY()
		TMap<FString, float> speedRate;			//이동속도의 비율 증감
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
	UPROPERTY(VisibleAnyWhere)
		TMap<FString, float> MaxHP;				//레벨업을 제외한 다른 요인들에 의해 증가되는 체력 딕셔너리
	UPROPERTY()
		TMap<FString, float> generateHealthPoint;	//레벨업을 제외한 다른 요인들에 의해 증가되는 체력 재생량 딕셔너리
	UPROPERTY()
		TMap<FString, float> MaxMP;			//레벨업을 제외한 다른 요인들에 의해 증가되는 마나 딕셔너리
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
		TMap<FString, int32> lifeSteal;			//레벨업을 제외한 다른 요인들에 의해 증가되는 흡혈 딕셔너리
	UPROPERTY()
		TMap<FString, int32> protectPower;		//레벨업을 제외한 다른 요인들에 의해 증가되는 회복 및 보호막 효과 딕셔너리
	UPROPERTY()
		TMap<FString, int32> endurance;			//레벨업을 제외한 다른 요인들에 의해 증가되는 인내심 딕셔너리
	UPROPERTY()
		TMap<FString, float> decreseDefencePoint;//방깍

	//DEBUG RED=MINION, BLUE = PLAYER
	E_Team team = E_Team::Blue;

public:		//Getter, Setter
	void SetLevel(int32 _level);
	int32 GetCurrentLevel();
	virtual void SetCurrentHP(float _hp);
	float GetCurrentHP();
	void IE_GenerateHP();
	float GetCurrentMP();
	int32 GetMoney();
	void GiveMoney(int32 _money);
	int32 GetExp();
	void GiveExp(int32 _exp);

	float GetRange();
	float GetAttackDamage();
	int32 GetCriticalRate();
	int32 GetCriticalDamage();
	float GetAttackSpeed();
	float GetMaxHP();
	float GetGenerateHealthPoint();
	float GetMaxMP();
	int32 GetLifeSteal();
	int32 GetProtectPower();
	float GetDefencePoint();
	float GetSpeed();

	void AddDecreseDefencePercent(FString name, float value, float time);
	float GetDecreseDefence();

	virtual void InitProperty();

public:	//Attack
	virtual void InitAttackProjectile();
	virtual void Attack(AActor* target) override;
	
	UFUNCTION()
		void InitProjectileProperty(ADummyProjectile* projectile);

	UFUNCTION()
		void LifeSteal(float damage);

protected:	//Move
	bool IsMoveVectorTrue = false;
	FVector moveDirection = FVector::ZeroVector;
	float moveForce = 0;
	int32 moveCnt = 0;

protected:	//TimerHandle
	FTimerHandle GenerateHPTimer;
 
public:		//Interface Implement
	UFUNCTION()
		virtual void Stun(float time) override;
	UFUNCTION()
		virtual void Stop(float time) override;
	UFUNCTION()
		virtual void Slow(float time, float value, FString key) override;
	UFUNCTION()
		virtual void Silence(float time) override;

	UFUNCTION()
		virtual void TakeDamage(float damage, AActor* damageCauser) override;
	UFUNCTION()
		virtual void RestoreHP(float value) override;
	UFUNCTION()
		virtual float GetHP();

	UFUNCTION()
		virtual void MoveVector(FVector Direction, float Force) override;

	virtual E_Team GetTeam();
};

