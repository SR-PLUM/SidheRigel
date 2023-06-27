// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "SidheRigel/Dummy/DummyProjectile.h"
#include "BlackWizardAttackProjectile.generated.h"

UCLASS()
class SIDHERIGEL_API ABlackWizardAttackProjectile : public ADummyProjectile
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	ABlackWizardAttackProjectile();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

protected:	//InitFunction
	virtual void SetProjectileMesh() override;
	virtual void SetProjectileMovementComponent() override;
};
