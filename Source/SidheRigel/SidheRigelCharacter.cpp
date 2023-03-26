// Copyright Epic Games, Inc. All Rights Reserved.

#include "SidheRigelCharacter.h"
#include "UObject/ConstructorHelpers.h"
#include "Camera/CameraComponent.h"
#include "Components/DecalComponent.h"
#include "Components/CapsuleComponent.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "GameFramework/PlayerController.h"
#include "GameFramework/SpringArmComponent.h"
#include "Materials/Material.h"
#include "Engine/World.h"

ASidheRigelCharacter::ASidheRigelCharacter()
{
	// Set size for player capsule
	GetCapsuleComponent()->InitCapsuleSize(42.f, 96.0f);

	// Don't rotate character to camera direction
	bUseControllerRotationPitch = false;
	bUseControllerRotationYaw = false;
	bUseControllerRotationRoll = false;

	// Configure character movement
	GetCharacterMovement()->bOrientRotationToMovement = true; // Rotate character to moving direction
	GetCharacterMovement()->RotationRate = FRotator(0.f, 640.f, 0.f);
	GetCharacterMovement()->bConstrainToPlane = true;
	GetCharacterMovement()->bSnapToPlaneAtStart = true;

	// Create a camera boom...
	CameraBoom = CreateDefaultSubobject<USpringArmComponent>(TEXT("CameraBoom"));
	CameraBoom->SetupAttachment(RootComponent);
	CameraBoom->SetUsingAbsoluteRotation(true); // Don't want arm to rotate when character does
	CameraBoom->TargetArmLength = 800.f;
	CameraBoom->SetRelativeRotation(FRotator(-60.f, 0.f, 0.f));
	CameraBoom->bDoCollisionTest = false; // Don't want to pull camera in when it collides with level

	// Create a camera...
	TopDownCameraComponent = CreateDefaultSubobject<UCameraComponent>(TEXT("TopDownCamera"));
	TopDownCameraComponent->SetupAttachment(CameraBoom, USpringArmComponent::SocketName);
	TopDownCameraComponent->bUsePawnControlRotation = false; // Camera does not rotate relative to arm

	// Activate ticking in order to update the cursor every frame.
	PrimaryActorTick.bCanEverTick = true;
	PrimaryActorTick.bStartWithTickEnabled = true;
}

void ASidheRigelCharacter::Tick(float DeltaSeconds)
{
    Super::Tick(DeltaSeconds);
}

void ASidheRigelCharacter::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	check(PlayerInputComponent);

	PlayerInputComponent->BindAction("SkillOne", IE_Pressed, this, &ASidheRigelCharacter::SkillOne);
	PlayerInputComponent->BindAction("SkillTwo", IE_Pressed, this, &ASidheRigelCharacter::SkillTwo);
	PlayerInputComponent->BindAction("SkillThree", IE_Pressed, this, &ASidheRigelCharacter::SkillThree);
	PlayerInputComponent->BindAction("SkillFour", IE_Pressed, this, &ASidheRigelCharacter::SkillFour);
}

void ASidheRigelCharacter::SkillOne()
{
	UE_LOG(LogTemp, Warning, TEXT("Q"));
}

void ASidheRigelCharacter::SkillTwo()
{
	UE_LOG(LogTemp, Warning, TEXT("W"));
}

void ASidheRigelCharacter::SkillThree()
{
	UE_LOG(LogTemp, Warning, TEXT("E"));
}

void ASidheRigelCharacter::SkillFour()
{
	UE_LOG(LogTemp, Warning, TEXT("R"));
}

void ASidheRigelCharacter::SetLevel(int32 _level)
{
	level = _level;

	if ((level != 19) && (level % 3 == 1 || level == 20))
	{
		//특성 띄우기
	}
}
