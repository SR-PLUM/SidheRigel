// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "../Interface/Damagable.h"
#include "../Interface/CCable.h"
#include "Dummy.generated.h"

UCLASS()
class SIDHERIGEL_API ADummy : public AActor, public IDamagable, public ICCable
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	ADummy();

protected:	//인터페이스 구현
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

	UFUNCTION()
		virtual void TakeDamage(float damage, AActor* damageCauser) override;
	UFUNCTION()
		virtual void RestoreHP(float value) override;
	
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

private:	//스텟
	UPROPERTY()
		TMap<FString, float> MaxHP;			//레벨업을 제외한 다른 요인들에 의해 증가되는 체력 딕셔너리
	UPROPERTY()
		float currentHP;

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

protected:	//매쉬
	UPROPERTY(VisibleAnywhere)
		UStaticMeshComponent* DummyMesh;
};
