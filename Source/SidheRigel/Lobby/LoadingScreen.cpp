// Fill out your copyright notice in the Description page of Project Settings.


#include "LoadingScreen.h"

void ULoadingScreen::Setup()
{
	this->AddToViewport();
}

void ULoadingScreen::Teardown()
{
	this->RemoveFromViewport();
}