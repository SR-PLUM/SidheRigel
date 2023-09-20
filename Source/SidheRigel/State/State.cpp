// Fill out your copyright notice in the Description page of Project Settings.


#include "State.h"

#include "StateMachine.h"
#include "SidheRigel/SidheRigelPlayerController.h"

State::State(StateMachine* StateMachine)
{
	stateMachine = StateMachine;
}

State::~State()
{
}
