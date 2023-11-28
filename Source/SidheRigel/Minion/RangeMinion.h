// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Minion.h"
#include "RangeMinion.generated.h"

/**
 * 
 */
UCLASS()
class SIDHERIGEL_API ARangeMinion : public AMinion
{
	GENERATED_BODY()
	
public:
	ARangeMinion();
	void BeginPlay() override;
	void InitProperty() override;
};
