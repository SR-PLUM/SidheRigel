// Fill out your copyright notice in the Description page of Project Settings.


#include "SilenceParticle.h"
#include "Components/WidgetComponent.h"

// Sets default values
ASilenceParticle::ASilenceParticle()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	if (!WidgetComponent)
	{
		WidgetComponent = CreateDefaultSubobject<UWidgetComponent>(TEXT("Widget"));

		static ConstructorHelpers::FClassFinder<UUserWidget> SilenceImage(TEXT("/Game/Heros/Common/WBP_Silence"));
		if (SilenceImage.Succeeded())
		{
			WidgetComponent->SetWidgetSpace(EWidgetSpace::Screen);

			WidgetComponent->SetWidgetClass(SilenceImage.Class);
			WidgetComponent->SetDrawSize(FVector2D(50, 50));
		}
	}
	
}

// Called when the game starts or when spawned
void ASilenceParticle::BeginPlay()
{
	Super::BeginPlay();
	
}

// Called every frame
void ASilenceParticle::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);


	if (target)
	{
		SetActorLocation(target->GetActorLocation() + FVector::UpVector * 180);
	}
}

