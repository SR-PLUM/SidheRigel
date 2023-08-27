// Fill out your copyright notice in the Description page of Project Settings.


#include "MenuWidget.h"

void UMenuWidget::SetMenuInterface(IMenuInterface* menuInterface)
{
	MenuInterface = menuInterface;
}

void UMenuWidget::Setup()
{
	this->AddToViewport();
}

void UMenuWidget::Teardown()
{
	this->RemoveFromViewport();
}