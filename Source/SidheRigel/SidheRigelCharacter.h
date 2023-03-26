// Copyright Epic Games, Inc. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Character.h"
#include "SidheRigelCharacter.generated.h"

UCLASS(Blueprintable)
class ASidheRigelCharacter : public ACharacter
{
	GENERATED_BODY()

public:
	ASidheRigelCharacter();

	// Called every frame.
	virtual void Tick(float DeltaSeconds) override;

	void SetupPlayerInputComponent(UInputComponent* PlayerInputComponent);

	/** Returns TopDownCameraComponent subobject **/
	FORCEINLINE class UCameraComponent* GetTopDownCameraComponent() const { return TopDownCameraComponent; }
	/** Returns CameraBoom subobject **/
	FORCEINLINE class USpringArmComponent* GetCameraBoom() const { return CameraBoom; }

private:
	/** Top down camera */
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = Camera, meta = (AllowPrivateAccess = "true"))
		class UCameraComponent* TopDownCameraComponent;

	/** Camera boom positioning the camera above the character */
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = Camera, meta = (AllowPrivateAccess = "true"))
		class USpringArmComponent* CameraBoom;

protected:	//Skill
	UFUNCTION()
		void SkillOne();
	UFUNCTION()
		void SkillTwo();
	UFUNCTION()
		void SkillThree();
	UFUNCTION()
		void SkillFour();

protected:	//Stat
	UPROPERTY()
		int32 level;										//레벨
	UPROPERTY()
		int32 experience;									//현재 보유하고 있는 경험치
	UPROPERTY()
		float range;										//사거리
	UPROPERTY()
		TMap<FString, float> additionalRange;				//레벨업을 제외한 다른 요인들에 의해 증가되는 사거리 딕셔너리
	UPROPERTY()
		float speed;										//이동속도
	UPROPERTY()
		TMap<FString, float> additionalSpeed;				//레벨업을 제외한 다른 요인들에 의해 증가되는 이동속도 딕셔너리
	UPROPERTY()
		float attackDamage;									//공격력
	UPROPERTY()
		TMap<FString, float> additionalAttackDamage;		//레벨업을 제외한 다른 요인들에 의해 증가되는 공격력 딕셔너리
	UPROPERTY()
		float abilityPower;									//주문력
	UPROPERTY()
		TMap<FString, float> additionalAbiltyPower;			//레벨업을 제외한 다른 요인들에 의해 증가되는 주문력 딕셔너리
	UPROPERTY()
		TMap<FString, int32> additionalCriticalRate;		//레벨업을 제외한 다른 요인들에 의해 증가되는 치명타 확률 딕셔너리
	UPROPERTY()
		TMap<FString, int32> additionalCriticalDamage;		//레벨업을 제외한 다른 요인들에 의해 증가되는 치명타 피해 딕셔너리
	UPROPERTY()
		float attackSpeed;									//공격속도
	UPROPERTY()
		TMap<FString, float> additionalAttackSpeed;			//레벨업을 제외한 다른 요인들에 의해 증가되는 치명타 피해 딕셔너리
	UPROPERTY()
		float healthPoint;									//체력
	UPROPERTY()
		TMap<FString, float> additionalHealthPoint;			//레벨업을 제외한 다른 요인들에 의해 증가되는 체력 딕셔너리
	UPROPERTY()
		float generateHealthPoint;							//체력 재생량
	UPROPERTY()
		TMap<FString, float> additionalGenerateHealthPoint;		//레벨업을 제외한 다른 요인들에 의해 증가되는 체력 재생량 딕셔너리
	UPROPERTY()
		float manaPoint;									//마나
	UPROPERTY()
		TMap<FString, float> additionalManaPoint;			//레벨업을 제외한 다른 요인들에 의해 증가되는 마나 딕셔너리
	UPROPERTY()
		float defencePoint;									//방어력
	UPROPERTY()
		TMap<FString, float> additionalDefencePoint;		//레벨업을 제외한 다른 요인들에 의해 증가되는 방어력 딕셔너리
	UPROPERTY()
		float magicResistPoint;								//마법저항력
	UPROPERTY()
		TMap<FString, float> additionalMagicResistPoint;	//레벨업을 제외한 다른 요인들에 의해 증가되는 마법저항력 딕셔너리
	UPROPERTY()
		float armorPenetration;								//물리관통력
	UPROPERTY()
		TMap<FString, float> additionalArmorPenetration;	//레벨업을 제외한 다른 요인들에 의해 증가되는 물리관통력 딕셔너리
	UPROPERTY()
		float magicPenetration;								//마법관통력
	UPROPERTY()
		TMap<FString, float> additionalMagicPenetration;	//레벨업을 제외한 다른 요인들에 의해 증가되는 마법관통력 딕셔너리
	UPROPERTY()
		int32 cooldownReduction;							//쿨타임감소
	UPROPERTY()
		TMap<FString, int32> additionalCooldownReduction;	//레벨업을 제외한 다른 요인들에 의해 증가되는 쿨타임감소 딕셔너리
	UPROPERTY()
		float lifeSteal;									//흡혈
	UPROPERTY()
		TMap<FString, float> additionalLifeSteal;			//레벨업을 제외한 다른 요인들에 의해 증가되는 흡혈 딕셔너리
	UPROPERTY()
		float protectPower;									//회복 및 보호막 효과
	UPROPERTY()
		TMap<FString, float> additionalProtectPower;		//레벨업을 제외한 다른 요인들에 의해 증가되는 회복 및 보호막 효과 딕셔너리
	UPROPERTY()
		float endurance;									//인내심
	UPROPERTY()
		TMap<FString, float> additionalEndurance;			//레벨업을 제외한 다른 요인들에 의해 증가되는 인내심 딕셔너리

public:		//Getter, Setter
	void SetLevel(int32 _level);
};

