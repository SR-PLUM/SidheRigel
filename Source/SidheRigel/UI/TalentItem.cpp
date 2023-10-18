// Fill out your copyright notice in the Description page of Project Settings.


#include "TalentItem.h"
#include "SidheRigel/SidheRigelCharacter.h"
#include "Components/Button.h"
#include "Components/TextBlock.h"

void UTalentItem::InitTalentItemRef(class ASidheRigelCharacter* Character, int32 Level, int32 Index)
{
	CharacterRef = Character;
	TalentLevel = Level;
	TalentIndex = Index;

	BtnTalentItem->OnClicked.AddDynamic(this, &UTalentItem::OnClicked);

	InitTalentItemInfo();
}

void UTalentItem::InitTalentItemInfo()
{
	TalentDescription = CharacterRef->talentListArray[TalentLevel].talentItems[TalentIndex].talentDescription;
	SetToolTipText(FText::FromString(TalentDescription));
	TalentNameText->SetText(FText::FromString(CharacterRef->talentListArray[TalentLevel].talentItems[TalentIndex].talentName));
}

void UTalentItem::OnClicked()
{
	CharacterRef->IsSelectedTalent[TalentLevel].IsSelected[TalentIndex] = true;
}
