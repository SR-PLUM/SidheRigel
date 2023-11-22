// Fill out your copyright notice in the Description page of Project Settings.


#include "SidheRigelCamera.h"
#include "Camera/CameraComponent.h"
#include "GameFramework/SpringArmComponent.h"

// Sets default values
ASidheRigelCamera::ASidheRigelCamera()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	// Create a camera boom (pulls in towards the player if there is a collision)
	CameraBoom = CreateDefaultSubobject<USpringArmComponent>(TEXT("CameraBoom"));
	CameraBoom->SetupAttachment(RootComponent);
	CameraBoom->SetUsingAbsoluteRotation(true); // Don't want arm to rotate when character does
	CameraBoom->TargetArmLength = 800.0f; // The camera follows at this distance behind the character
	CameraBoom->SetRelativeRotation(FRotator(-60.f, 0.f, 0.f));
	CameraBoom->bDoCollisionTest = false;
	CameraBoom->bUsePawnControlRotation = true; // Rotate the arm based on the controller

	// Create a follow camera
	FollowCamera = CreateDefaultSubobject<UCameraComponent>(TEXT("FollowCamera"));
	FollowCamera->SetupAttachment(CameraBoom, USpringArmComponent::SocketName); // Attach the camera to the end of the boom and let the boom adjust to match the controller orientation
	FollowCamera->bUsePawnControlRotation = false; // Camera does not rotate relative to arm

	// Camera class will be constructed with target set to nullptr
	TargetToFollow = nullptr;
}

// Called when the game starts or when spawned
void ASidheRigelCamera::BeginPlay()
{
	Super::BeginPlay();

	// Get the rotation for the camera (it is set in the MyProjectCharacter.cpp file in the void AMyProjectCharacter::Look(const FInputActionValue& Value) method based on the input from player)
	FRotator ControlRotation = PlayerController->GetControlRotation();
	// Apply the rotation to the camera actor
	SetActorRotation(ControlRotation);

	// Check if a target is set
	if (TargetToFollow) {
		// Get location of the target
		FVector TargetLocation = TargetToFollow->GetActorLocation();
		// Set camera location to be equal to the location of the target (this will make sure our camera actor is always at the position of the target)
		SetActorLocation(TargetLocation);
	}
	
}

// Called every frame
void ASidheRigelCamera::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

