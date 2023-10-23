// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "AIController.h"

#include "../State/StateMachine.h"

#include "MinionAIController.generated.h"

/**
 * 
 */
UCLASS()
class SIDHERIGEL_API AMinionAIController : public AAIController
{
	GENERATED_BODY()
	
public:
	virtual void OnMoveCompleted(FAIRequestID RequestID, const FPathFollowingResult& Result) override;
};
