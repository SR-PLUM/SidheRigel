// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"

#include "../Interface/Damagable.h"
#include "../Interface/Team.h"

#include "Nexus.generated.h"

UCLASS()
class SIDHERIGEL_API ANexus : public AActor, public IDamagable, public ITeam
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	ANexus();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

protected:
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
	float MaxHP = 3000;

protected:
	UPROPERTY()
		class UWidgetComponent* NexusWidget;
	UPROPERTY()
		class UHPUI* NexusUIRef;

	void InitNexusWidget();
	void InitNexusUI();

	AActor* GetCloseEnemy();

	float range = 200.f;
public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	UFUNCTION()
		void OnEnterEnemy(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult);
	UFUNCTION()
		void OnExitEnemy(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex);

public:
	TSubclassOf<class ANexusAttackProjectile> projectileClass;

	UPROPERTY(EditAnywhere)
		class UStaticMeshComponent* muzzleLocation;

	UPROPERTY()
		TArray<class AActor*> attackList;

	UPROPERTY()
		AActor* currentTarget;

	UPROPERTY()
		float attackDelay = 0.f;
	UPROPERTY()
		float maxAttackDelay = 1.5f;

	UPROPERTY()
		float damage = 150.f;

	UPROPERTY()
		bool bIsTowerDestroyed = false;
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
	TSubclassOf<class ANexusDestroyParticle> particleClass;
};