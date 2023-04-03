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

protected:	//�������̽� ����
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

	virtual void TakeDamage(float damage) override;
	virtual void RestoreHP(float value) override;
	
	virtual void Stun(float time) override;
	virtual void Stop(float time) override;
	virtual void Slow(float time, float value) override;
	virtual void Silence(float time) override;
	virtual void Airborne(float time) override;

private:	//����
	UPROPERTY()
		TMap<FString, float> MaxHP;			//�������� ������ �ٸ� ���ε鿡 ���� �����Ǵ� ü�� ��ųʸ�
	UPROPERTY()
		float currentHP;

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

protected:	//�Ž�
	UPROPERTY(VisibleAnywhere)
		UStaticMeshComponent* DummyMesh;
};
