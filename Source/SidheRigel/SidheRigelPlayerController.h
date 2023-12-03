// Copyright Epic Games, Inc. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "Templates/SubclassOf.h"
#include "GameFramework/PlayerController.h"

#include "State/StateMachine.h"

#include "SidheRigelPlayerController.generated.h"

/** Forward declaration to improve compiling times */
class UNiagaraSystem;

UCLASS()
class ASidheRigelPlayerController : public APlayerController
{
	GENERATED_BODY()

public:
	ASidheRigelPlayerController();

	virtual void BeginPlay() override;

	/** Time Threshold to know if it was a short press */
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Input)
	float ShortPressThreshold;

	/** FX Class that we will spawn when clicking */
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Input)
	UNiagaraSystem* FXCursor;

	StateMachine* stateMachine;

	FHitResult GetHitResult();

protected:
	/** True if the controlled character should navigate to the mouse cursor. */
	uint32 bMoveToMouseCursor : 1;

	// Begin PlayerController interface
	virtual void PlayerTick(float DeltaTime) override;
	virtual void SetupInputComponent() override;
	// End PlayerController interface

	/** Input handlers for SetDestination action. */
	void OnSetDestinationReleased();

	void ClickedRightMouseButton();
	void ClickedLeftMouseButton();

	void PressedQButton();
	void PressedWButton();
	void PressedEButton();
	void PressedRButton();
	
	void PressedYButton();
public:
	FORCEINLINE UClass* GetPlayerPawnClass() { return MyPawnClass; }

protected:
	UPROPERTY(Replicated)
		TSubclassOf<APawn> MyPawnClass;
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly)
		TSubclassOf<class AColdCharacter> ColdPawn;
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly)
		TSubclassOf<class AFairyWingCharacter> FairyWingPawn;
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly)
		TSubclassOf<class AKerunCharacter> KerunPawn;

	UFUNCTION(Reliable, Client)
		void DeterminePawnClass();

	UFUNCTION(Reliable, Server)
		void ServerSetPawn(TSubclassOf<APawn> InPawnClass);

private:
	bool bInputPressed; // Input is bring pressed
	float FollowTime; // For how long it has been pressed

public: //Camera
	
	UFUNCTION(Reliable, Server)
		void SetSRCamera(APawn* aPawn, class ASidheRigelPlayerController* controller);

	UFUNCTION(Reliable, Client)
		void SetSRCameraInClient(APawn* aPawn, class ASidheRigelPlayerController* controller);

	UFUNCTION(Reliable, Client)
		void SpawnSRCamera(APawn* aPawn, class ASidheRigelPlayerController* controller);

protected:
	// Reference to our camera
	UPROPERTY(VisibleAnyWhere, Replicated)
	class ASidheRigelCamera* SRCamera;
	
	
	
	
 public:
	// Setter to set camera from another scripts
	FORCEINLINE void SetCamera(class ASidheRigelCamera* NewCamera) { SRCamera = NewCamera; }

//protected:
//	// Declare an override for the OnPossess method. This method will be called when the controller possesses will possess our player at the start of the game
//	virtual void OnPossess(APawn* aPawn) override;
//	

};


