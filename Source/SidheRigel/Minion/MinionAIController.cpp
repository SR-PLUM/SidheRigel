// Fill out your copyright notice in the Description page of Project Settings.


#include "MinionAIController.h"

#include "Minion.h"

void AMinionAIController::OnMoveCompleted(FAIRequestID RequestID, const FPathFollowingResult& Result)
{
	UE_LOG(LogTemp, Warning, TEXT("MOVE COMPLETED"));
}
