// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "Camera/CameraActor.h"
#include "SidheRigelCamera.generated.h"

UCLASS()
class SIDHERIGEL_API ASidheRigelCamera : public ACameraActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	ASidheRigelCamera();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

public:
	/** Camera boom positioning the camera behind the character */
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = Camera, meta = (AllowPrivateAccess = "true"))
		class USpringArmComponent* CameraBoom;

	/** Follow camera */
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = Camera, meta = (AllowPrivateAccess = "true"))
		class UCameraComponent* FollowCamera;

protected:
	// Player controller
	UPROPERTY()
	class APlayerController* PlayerController;

public:
	// Setter for the player controller
	FORCEINLINE void SetPlayerController(class APlayerController* NewPlayerController) { PlayerController = NewPlayerController; }

protected:
	// An actor the camera will follow
	UPROPERTY()
	class AActor* TargetToFollow;

public:
	FORCEINLINE void SetTargetToFollow(AActor* NewTargetToFollow) { TargetToFollow = NewTargetToFollow; UE_LOG(LogTemp, Warning, TEXT("Target :: %s"), *TargetToFollow->GetName()) }

protected:
	UPROPERTY()
	bool IsCameraFixed = true;

	float ScreenX;
	float ScreenY;

	UPROPERTY(EditAnywhere)
	float CameraMoveAmount = 15.f;

public:
	void SwitchIsCameraFixed();
	void SetCameraInActorLocation();
	bool GetIsCameraFixed();
	void SetScreenSize(float X, float Y);
	float GetScreenX();
	float GetScreenY();

	UFUNCTION(Reliable , Client)
	void MoveCameraX(float Axis);

	UFUNCTION(Reliable, Client)
	void MoveCameraY(float Axis);

};
