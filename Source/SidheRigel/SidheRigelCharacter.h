// Copyright Epic Games, Inc. All Rights Reserved.

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
#include "Character/TalentList.h"

#include "SidheRigelCharacter.generated.h"

USTRUCT()
struct FIsSelectedTalentItem
{
	GENERATED_USTRUCT_BODY()

public:

	TArray<bool> IsSelected;

	bool operator[] (int32 i) {
		return IsSelected[i];
	}

	void Add(bool b) {
		IsSelected.Add(b);
	}
};

USTRUCT()
struct FReduceHeal
{
	GENERATED_USTRUCT_BODY()

public:
	int32 debuffAmout;
	float debuffDuration;
};

UCLASS(Blueprintable)
class ASidheRigelCharacter : public ACharacter, public IAttackable, public ICCable, public IDamagable, public IMovable, public ITeam
{
	GENERATED_BODY()

	friend class ASidheRigelPlayerController;

public:
	ASidheRigelCharacter();

	virtual void BeginPlay();
	virtual void Tick(float DeltaTime) override;

	virtual void PossessedBy(AController* NewController) override;

	void SetupPlayerInputComponent(UInputComponent* PlayerInputComponent);

	/** Returns TopDownCameraComponent subobject **/
	//FORCEINLINE class UCameraComponent* GetTopDownCameraComponent() const { return TopDownCameraComponent; }
	/** Returns CameraBoom subobject **/
	//FORCEINLINE class USpringArmComponent* GetCameraBoom() const { return CameraBoom; }

	UPROPERTY(replicated, EditAnywhere)
		ASidheRigelPlayerController* sidheRigelController;

	UFUNCTION(reliable, client)
		void SetClientStateMachine();
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

	UPROPERTY(VisibleAnywhere)
		class USphereComponent* detectRange;

public://SERVER
	UFUNCTION(Reliable, Server)
		void Server_MoveToPoint(FVector Location);

	UFUNCTION(Reliable, server)
		void Server_MoveToStartLocation(FVector Location);

public:
	UPROPERTY(VisibleAnywhere)
		class UStaticMeshComponent* skillRange;

public:	//change target when attack enemy hero
	UFUNCTION()
		virtual void OnEnterEnemy(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult);

	UFUNCTION()
		virtual void OnExitEnemy(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex);

	TArray<class AActor*> InRangeActors;

	void ChangeTarget();

public:	//Skill
	UPROPERTY()
		TMap<TEnumAsByte<E_SkillState>, class USkill*> skills;

	virtual void UseSkill(FHitResult HitResult, E_SkillState SkillState);

public: //Talent
	TArray<FTalentList> talentListArray;

	TArray<FIsSelectedTalentItem> IsSelectedTalent;

	TMap<int32, class UTalentUI*> TalentUIArray;

	UPROPERTY()
		TSubclassOf<class UUserWidget> TalentUIWidget;

	UPROPERTY()
		TSubclassOf<class UUserWidget> TalentItemWidget;

	void InitTalentLIst();

	void RemoveTalentUI(int32 Index);

	void InitTalentWidget();

	void DisplayTalentList(int32 Index);

public: //UI

	TSubclassOf<UUserWidget> InGameUIWidget;

	UPROPERTY(VisibleAnywhere, Category = "InGameUI")
		class UInGameUI* InGameUI;

	void InitInGameUIWidget();
	void InitInGameUI();

	UPROPERTY()
		class UWidgetComponent* StatWidget;

	UPROPERTY()
		class UStatSummary* StatSummary;

	UPROPERTY()
		TSubclassOf<class UUserWidget> DeathUIWidget;

	UPROPERTY()
		class UDeathTime* DeathUI;

	void InitStatWidget();
	void InitStatSummary();
	void TurnOffStatUI();
	void TurnOnStatUI();

	void SetUISkillCoolDown(E_SkillState SkillState, float Percentage, float CurrentCoolDown);
	void ClearUISkillCoolDown(E_SkillState SkillState);

	void InitDeathUIWidget();
	void SpawnDeathUI();
	void SetDeathUI(float CurrentCoolDown);
	void ClearDeathUI();

public:
	TSubclassOf<AActor> deathActorClass;

	void InitDeathActorClass();
	void SpawnDeathActor();

public:	//CC Particle
	UPROPERTY(EditDefaultsOnly)
		TSubclassOf<class AStunParticle> stunParticleClass;
	UFUNCTION()
		void SpawnStunParticle();
	UPROPERTY()
		class AStunParticle* stunParticle;
	UFUNCTION()
		void RemoveStunParticle();

	UPROPERTY(EditDefaultsOnly)
		TSubclassOf<class ASlowParticle> slowParticleClass;
	UFUNCTION()
		void SpawnSlowParticle();
	UPROPERTY()
		class ASlowParticle* slowParticle;
	UFUNCTION()
		void RemoveSlowParticle();

	UPROPERTY(EditDefaultsOnly)
		TSubclassOf<class AStopParticle> stopParticleClass;
	UFUNCTION()
		void SpawnStopParticle();
	UPROPERTY()
		class AStopParticle* stopParticle;
	UFUNCTION()
		void RemoveStopParticle();

	UPROPERTY(EditDefaultsOnly)
		TSubclassOf<class ASilenceParticle> silenceParticleClass;
	UFUNCTION()
		void SpawnSilenceParticle();
	UPROPERTY()
		class ASilenceParticle* silenceParticle;
	UFUNCTION()
		void RemoveSilenceParticle();

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
		float rangeBase;						//기본적으로 가지는 사거리
	UPROPERTY()
		TMap<FString, float> speed;				//레벨업을 제외한 다른 요인들에 의해 증가되는 이동속도 딕셔너리
	UPROPERTY()
		float speedBase;
	UPROPERTY()
		TMap<FString, float> speedRate;			//이동속도의 비율 증감
	UPROPERTY()
		TMap<FString, float> attackDamage;		//레벨업을 제외한 다른 요인들에 의해 증가되는 공격력 딕셔너리
	UPROPERTY()
		float attackDamageBase;
	UPROPERTY()
		TMap<FString, float> abiltyPower;		//레벨업을 제외한 다른 요인들에 의해 증가되는 주문력 딕셔너리
	UPROPERTY()
		TMap<FString, int32> criticalRate;		//레벨업을 제외한 다른 요인들에 의해 증가되는 치명타 확률 딕셔너리
	UPROPERTY()
		int32 criticalRateBase;
	UPROPERTY()
		TMap<FString, int32> criticalDamage;	//레벨업을 제외한 다른 요인들에 의해 증가되는 치명타 피해 딕셔너리
	UPROPERTY()
		int32 criticalDamageBase;
	UPROPERTY()
		TMap<FString, float> attackSpeed;		//레벨업을 제외한 다른 요인들에 의해 증가되는 치명타 피해 딕셔너리
	UPROPERTY()
		float attackSpeedBase;
	UPROPERTY(VisibleAnyWhere)
		TMap<FString, float> MaxHP;				//레벨업을 제외한 다른 요인들에 의해 증가되는 체력 딕셔너리
	UPROPERTY()
		float MaxHPBase;
	UPROPERTY()
		TMap<FString, float> generateHealthPoint;	//레벨업을 제외한 다른 요인들에 의해 증가되는 체력 재생량 딕셔너리
	UPROPERTY()
		float generateHealthPointBase;
	UPROPERTY()
		TMap<FString, float> generateManaPoint;	//레벨업을 제외한 다른 요인들에 의해 증가되는 마나 재생량 딕셔너리
	UPROPERTY()
		float generateManaPointBase;
	UPROPERTY()
		TMap<FString, float> MaxMP;			//레벨업을 제외한 다른 요인들에 의해 증가되는 마나 딕셔너리
	UPROPERTY()
		float MaxMPBase;
	UPROPERTY()
		TMap<FString, float> defencePoint;		//레벨업을 제외한 다른 요인들에 의해 증가되는 방어력 딕셔너리
	UPROPERTY()
		float defencePointBase;
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
		int32 lifeStealBase;
	UPROPERTY()
		TMap<FString, int32> protectPower;		//레벨업을 제외한 다른 요인들에 의해 증가되는 회복 및 보호막 효과 딕셔너리
	UPROPERTY()
		int32 protectPowerBase;
	UPROPERTY()
		TMap<FString, int32> endurance;			//레벨업을 제외한 다른 요인들에 의해 증가되는 인내심 딕셔너리
	UPROPERTY()
		TMap<FString, float> decreseDefencePoint;//방깍
	UPROPERTY()
		int32 reduceMyHeal;						//본인에게 적용중인 치유감소 (20% == 20) (높은 값이 우선적용)
	UPROPERTY()
		float reduceHealDuration;				//치유감소 남은 시간
	UPROPERTY()
		TMap<FString, FReduceHeal> reduceOtherHeal;	//상대방에게 적용시킬 치유감소 (20% == 20)
	UPROPERTY()
		float barrierAmount =0.f;				//보호막

	UPROPERTY(replicated, EditAnyWhere)
		TEnumAsByte<E_Team> team = E_Team::Team_Null;

private:
	float barrierDuration = 4.f;

public:		//Getter, Setter
	void SetLevel(int32 _level);
	int32 GetCurrentLevel();
	virtual void SetCurrentHP(float _hp);
	float GetCurrentHP();
	void IE_GenerateHP();
	void UseMana(float UseMP);
	float GetCurrentMP();
	void IE_GenerateMP();
	int32 GetMoney();
	void GiveMoney(int32 _money);
	int32 GetExp();
	virtual void GiveExp(int32 _exp);
	int32 GetMaxExp();
	void LevelUp();

	float GetRange();
	float GetAttackDamage();
	void AddAttackDamage(FString name, float value);
	void RemoveAttackDamage(FString name);
	int32 GetCriticalRate();
	int32 GetCriticalDamage();
	float GetAttackSpeed();
	void AddAttackSpeed(FString name, float value);
	void RemoveAttackSpeed(FString name);

	float GetMaxHP();
	float GetGenerateHealthPoint();
	float GetMaxMP();
	float GetGenerateManaPoint();
	int32 GetLifeSteal();
	int32 GetProtectPower();
	int32 GetEndurance();
	void AddDefencePoint(FString name, float value, float time);
	float GetDefencePoint();
	void AddSpeed(FString name, float value, float time);
	float GetSpeed();

	void AddDecreseDefencePercent(FString name, float value, float time);
	float GetDecreseDefence();

	void SetReduceMyHeal(int32 reduceHeal, float duration);
	FReduceHeal GetReduceOtherHeal();

	void AddReduceOtherHeal(FString name, int32 reduceHeal, float duration);
	void RemoveReduceOtherHeal(FString name);

	void AddBarrierAmount(float value);
	void DecreaseBarrierAmount(float value);

	float GetRemainDieCooldown();

	virtual void InitProperty();

	UPROPERTY(BlueprintReadOnly)
		bool isDie = false;
	UPROPERTY() //During Die
		float DieTime = 0;
	UPROPERTY()
		FVector spawnLocation;

	UPROPERTY()
		float frontDelay = 0.1f;

public:	//Attack
	virtual void InitAttackProjectile();

	UFUNCTION(reliable, server)
		virtual void Attack(AActor* target) override;
	
	UFUNCTION()
		void InitProjectileProperty(ADummyProjectile* projectile);

	UFUNCTION()
		void LifeSteal(float damage);

protected:	//TimerHandle
	FTimerHandle GenerateHPTimer;
	FTimerHandle BarrierTimer;
 
public:		//Interface Implement
	UFUNCTION()
		virtual void Stun(float time) override;
	UFUNCTION()
		virtual void Stop(float time) override;
	UFUNCTION()
		virtual void Slow(float time, float value, FString key) override;
	UFUNCTION()
		virtual void Silence(float time) override;
	UPROPERTY() //During Stun
		float stunTime = 0;
	UPROPERTY() //During Stop
		float stopTime = 0;
	UPROPERTY() //During Silence
		float silenceTime = 0;

	UFUNCTION()
		virtual void TakeDamage(float damage, AActor* damageCauser) override;
	UFUNCTION()
		virtual void RestoreHP(float value) override;
	UFUNCTION()
		virtual void RestoreMP(float value);
	UFUNCTION()
		virtual float GetHP();
	UFUNCTION()
		virtual float GetMP();

	UFUNCTION()
		virtual void MoveVector(FVector Direction, float Force) override;

	virtual E_Team GetTeam();

public:	//FairyWing E Skill Talent
	bool isStopMarkAlreadyHit = false;
	bool isBombMarkAlreadyHit = false;

public: //FairyWing W Skill Talent
	bool isWSkillAlreadyHit = false;
};

