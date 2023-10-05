// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Engine/StaticMeshActor.h"
#include "WayPoint.generated.h"

/**
 * 
 */
UCLASS()
class SIDHERIGEL_API AWayPoint : public AStaticMeshActor
{
	GENERATED_BODY()
public:
	UPROPERTY(EditAnywhere)
		int32 wayPointOrder;
};
