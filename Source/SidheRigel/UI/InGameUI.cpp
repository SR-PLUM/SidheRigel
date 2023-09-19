// Fill out your copyright notice in the Description page of Project Settings.


#include "InGameUI.h"
#include "CharacterStatus.h"
#include "CombatSummary.h"
#include "SidheRigel/SidheRigelGameInstance.h"

//void UInGameUI::NativeConstruct()
//{
//	Super::NativeConstruct();
//
//	USidheRigelGameInstance* GameInstance = GetWorld()->GetGameInstance<USidheRigelGameInstance>();
//
//	InitCharacterData(GameInstance->CharacterNum);
//}

void UInGameUI::InitCharacterData(E_Character Character)
{
	CharacterStatus = Cast<UCharacterStatus>(CreateWidget(GetWorld(), UCharacterStatus::StaticClass()));
	CombatSummary = Cast<UCombatSummary>(CreateWidget(GetWorld(), UCombatSummary::StaticClass()));


	if (CharacterStatus != nullptr)
	{
		CharacterStatus->InitIconImage(Character);
	}
	else
	{
		UE_LOG(LogTemp, Warning, TEXT("CharacterStatus is null"));
	}
	

	
}
