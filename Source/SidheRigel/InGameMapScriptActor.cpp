// Fill out your copyright notice in the Description page of Project Settings.


#include "InGameMapScriptActor.h"
#include "UI/InGameUI.h"
#include "SidheRigel/SidheRigelGameInstance.h"
//#include "SidheRigel/SidheRigelPlayerController.h"
#include "Kismet/GameplayStatics.h"

AInGameMapScriptActor::AInGameMapScriptActor()
{
	PrimaryActorTick.bCanEverTick = true;

	static ConstructorHelpers::FClassFinder<UUserWidget> InGameUIBPClass(TEXT("/Game/UIBlueprints/InGameUI/WBP_InGameUI"));
	if (InGameUIBPClass.Class == nullptr)
		return;

	InGameUIWidget = InGameUIBPClass.Class;

	if (InGameUIWidget == nullptr) return;

	UE_LOG(LogTemp, Warning, TEXT("InGameUIWidget"))
}

UInGameUI* AInGameMapScriptActor::GetInGameUI()
{
	return InGameUI;
}

void AInGameMapScriptActor::BeginPlay()
{
	Super::BeginPlay();

	UE_LOG(LogTemp, Warning, TEXT("InGame Map ScriptActor"))

	//InGameUI = Cast<UInGameUI>(CreateWidget(GetGameInstance(), InGameUIWidget));

	FSoftClassPath InGameUIWidgetClassRef(TEXT("WidgetBlueprint'/Game/UIBlueprints/InGameUI/WBP_InGameUI.WBP_InGameUI_C'"));
	if (UClass* InGameUIWidgetClass = InGameUIWidgetClassRef.TryLoadClass<UUserWidget>()) {
		InGameUI = CreateWidget<UInGameUI>(GetWorld(), InGameUIWidgetClass);

		if (InGameUI != nullptr)
		{
			USidheRigelGameInstance* GameInstance = GetWorld()->GetGameInstance<USidheRigelGameInstance>();

			InGameUI->InitCharacterData(GameInstance);

			InGameUI->AddToViewport();
		}
		else
		{
			UE_LOG(LogTemp, Warning, TEXT("InGameUI is null"))
		}

		//InGameUI->AddToViewport();
	}
	else
	{
		UE_LOG(LogTemp, Warning, TEXT("Load Failed"))
	}
	

	/*if (InGameUI != nullptr)
	{
		USidheRigelGameInstance* GameInstance = GetWorld()->GetGameInstance<USidheRigelGameInstance>();

		InGameUI->InitCharacterData(GameInstance->CharacterNum);

		InGameUI->AddToViewport();
	}
	else
	{
		UE_LOG(LogTemp, Warning, TEXT("InGameUI is null"))
	}*/
	
}

void AInGameMapScriptActor::Tick(float DeltaSeconds)
{
	Time += DeltaSeconds;
}