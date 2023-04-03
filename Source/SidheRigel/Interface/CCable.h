// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "UObject/Interface.h"
#include "CCable.generated.h"

// This class does not need to be modified.
UINTERFACE(MinimalAPI)
class UCCable : public UInterface
{
	GENERATED_BODY()
};

/**
 * 
 */
class SIDHERIGEL_API ICCable
{
	GENERATED_BODY()

	// Add interface functions to this class. This is the class that will be inherited to implement this interface.
public:
	virtual void Stun(float time) = 0;
	virtual void Stop(float time) = 0;
	virtual void Slow(float time, float value) = 0;
	virtual void Silence(float time) = 0;
	virtual void Airborne(float time) = 0;
};
