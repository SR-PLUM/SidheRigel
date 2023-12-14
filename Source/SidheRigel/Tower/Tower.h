// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"

#include "../Interface/Damagable.h"
#include "../Interface/Team.h"

#include "Tower.generated.h"

UCLASS()
class SIDHERIGEL_API ATower : public AActor, public IDamagable, public ITeam
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	ATower();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

protected:
	UPROPERTY(EditAnywhere)
		class USphereComponent* rangeArea;

	UPROPERTY(EditAnywhere)
		class UParticleSystemComponent* destroyParticle;

	UPROPERTY(EditAnywhere)
		class USoundBase* destroySound;

	UPROPERTY(EditAnywhere)
		class USoundBase* AttackSound;

	UPROPERTY(VisibleAnywhere)
		class UStaticMeshComponent* mesh;

	UPROPERTY(EditAnywhere)
		TEnumAsByte<E_Team> team = E_Team::Team_Null;

	float HP;
	float MaxHP = 3300;

	float range = 500;

protected:
	UPROPERTY()
		class UWidgetComponent* TowerWidget;
	UPROPERTY()
		class UHPUI* TowerUIRef;

	void InitTowerWidget();
	void InitTowerUI();

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	UFUNCTION()
		void OnEnterEnemy(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult);
	UFUNCTION()
		void OnExitEnemy(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex);

	AActor* GetCloseEnemy();
public:
	TSubclassOf<class ATowerAttackProjectile> projectileClass;

	UPROPERTY(EditAnywhere)
		class UStaticMeshComponent* muzzleLocation;

	UPROPERTY()
		TArray<class AActor*> attackList;

	UPROPERTY()
		AActor* currentTarget;

	UPROPERTY()
		float attackDelay = 0.f;
	UPROPERTY()
		float maxAttackDelay = 1.2f;

	UPROPERTY()
		float damage = 170;

	UPROPERTY(EditAnywhere)
		class ANexus* nexus;
public:
	UFUNCTION()
		virtual void TakeDamage(float _damage, AActor* damageCauser);
	UFUNCTION()
		virtual void RestoreHP(float value);
	UFUNCTION()
		virtual float GetHP();

	UFUNCTION()
		virtual E_Team GetTeam();

private:
	TSubclassOf<class ATowerDestroyParticle> particleClass;
};
