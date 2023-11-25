// Fill out your copyright notice in the Description page of Project Settings.


#include "TalentItem.h"
#include "SidheRigel/SidheRigelCharacter.h"
#include "Components/Button.h"
#include "Components/TextBlock.h"
#include "TalentToolTip.h"
#include "Styling/SlateBrush.h"

UTalentItem::UTalentItem(const FObjectInitializer& ObjectInitializer) : UUserWidget(ObjectInitializer)
{

	InitToolTip();
}

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
	FString ImagePath = CharacterRef->talentListArray[TalentLevel].talentItems[TalentIndex].imgPath;
	UTexture2D* TalentImage = Cast<UTexture2D>(StaticLoadObject(UTexture2D::StaticClass(), NULL, *ImagePath));

	BtnTalentItem->WidgetStyle.Normal.SetResourceObject(TalentImage);
	BtnTalentItem->WidgetStyle.Hovered.SetResourceObject(TalentImage);
	BtnTalentItem->WidgetStyle.Pressed.SetResourceObject(TalentImage);

	TalentDescription = CharacterRef->talentListArray[TalentLevel].talentItems[TalentIndex].talentDescription;
	//TalentNameText->SetText(FText::FromString(CharacterRef->talentListArray[TalentLevel].talentItems[TalentIndex].talentName));
	BtnTalentItem->SetToolTipText(FText::FromString(TalentDescription));

	/*if (!TalentToolTipWidget)
	{
		UE_LOG(LogTemp, Warning, TEXT("TalentToolTipWidget is null"));
		return;
	}

	ToolTip = CreateWidget<UTalentToolTip>(this, TalentToolTipWidget);

	ToolTip->DescriptionText->SetText(FText::FromString(TalentDescription));
	BtnTalentItem->SetToolTip(ToolTip);*/

}

void UTalentItem::OnClicked()
{
	CharacterRef->IsSelectedTalent[TalentLevel].IsSelected[TalentIndex] = true;
	CharacterRef->RemoveTalentUI(TalentLevel);
}

void UTalentItem::InitToolTip()
{
	ConstructorHelpers::FClassFinder<UUserWidget> TalentToolTipClass(TEXT("/Game/UIBlueprints/InGameUI/WBP_TalentToolTip"));
	if (TalentToolTipClass.Class == nullptr)
		return;

	TalentToolTipWidget = TalentToolTipClass.Class;
}