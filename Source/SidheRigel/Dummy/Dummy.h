// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "../Interface/Damagable.h"
#include "../Interface/CCable.h"
#include "../Interface/Movable.h"
#include "Dummy.generated.h"

UCLASS()
class SIDHERIGEL_API ADummy : public AActor, public IDamagable, public ICCable, public IMovable
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	ADummy();

protected:	//Interface Implement
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

	UFUNCTION()
		virtual void MoveVector(FVector Direction, float Force) override;

private:	//Stat
	UPROPERTY()
		TMap<FString, float> MaxHP;
	UPROPERTY()
		float currentHP;

protected:	//Move
	bool IsMoveVectorTrue = false;
	FVector moveDirection = FVector::ZeroVector;
	float moveForce = 0;
	int32 moveCnt = 0;

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

protected:	//매쉬
	UPROPERTY(VisibleAnywhere)
		UStaticMeshComponent* DummyMesh;
};
