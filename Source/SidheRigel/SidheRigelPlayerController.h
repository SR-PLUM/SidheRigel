// Copyright Epic Games, Inc. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "Templates/SubclassOf.h"
#include "GameFramework/PlayerController.h"
#include "Enum/E_SkillState.h"
#include "Enum/E_Team.h"

#include "SidheRigelPlayerController.generated.h"

/** Forward declaration to improve compiling times */
class UNiagaraSystem;

UCLASS()
class ASidheRigelPlayerController : public APlayerController
{
	GENERATED_BODY()

	//Basic Task
public:
	ASidheRigelPlayerController();
	virtual void BeginPlay() override;
protected:
	// Begin PlayerController interface
	virtual void OnPossess(APawn* aPawn) override;
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
	void PressedSpaceButton();
	void PressedAButton();
	void PressedSButton();

	//Set Pawn
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

	TSubclassOf<class AAISidheRigelCharacter> AIClass;

	UFUNCTION(Reliable, Client)
		void DeterminePawnClass();
	UFUNCTION(Reliable, Server)
		void ServerSetPawn(TSubclassOf<APawn> InPawnClass);

	//StateMachine
public:
	/*UPROPERTY(replicated, EditAnywhere)
		UStateMachine* stateMachine;*/
	FTimerHandle stateUpdateTimer;
	void IE_Update();
	float deltaTime = 0.1;

	void InitializeState();
public:
	UPROPERTY()
		class UState* currentState;
	UPROPERTY()
		class UState* previousState;
public:
	UPROPERTY()
	class UState* Idle;
	UPROPERTY()
	class UState* MoveToAttack;
	UPROPERTY()
	class UState* Move;
	UPROPERTY()
	class UState* AttackWait;
	UPROPERTY()
	class UState* Attack;
	UPROPERTY()
	class UState* UseSkill;
//Special State
	UPROPERTY()
	class UState* Die;
public:
	class ASidheRigelCharacter* myCharacter;
	AActor* target;
	FVector location;

	bool bAttackReady = false;
	bool bSkillReady = false;

	TEnumAsByte<E_SkillState> currentSkill = E_SkillState::Skill_Null;

public:	//Timer
	UPROPERTY() //Delay between attack to attack
		float attackDelay = 0;
	UPROPERTY() //Delay between cast to attack
		float frontDelay = 0;
	UPROPERTY() //Delay During use Skill
		float skillDelay = 0;

	void ChangeState(class UState* NextState);
	void ChangePreviousState();
	UState* GetCurrentState();

	void OnKeyboard(E_SkillState SkillState);
	void HasAttackEnemy();
	void ChangeCurrentSkill(E_SkillState SkillState);

	FHitResult GetHitResult();
	AActor* GetCloseActorToMouse();

	//SetTeam
protected:
	UPROPERTY(replicated, EditAnyWhere)
		TEnumAsByte<E_Team> myTeam;
	UFUNCTION(reliable, server)
		void ServerSetTeam(E_Team team);

public: //Camera
	UFUNCTION(Reliable, Server)
		void SetSRCamera(APawn* aPawn, class ASidheRigelPlayerController* controller);
	UFUNCTION(Reliable, Client)
		void SetSRCameraInClient(APawn* aPawn, class ASidheRigelPlayerController* controller);
	UFUNCTION(Reliable, Client)
		void SpawnSRCamera(APawn* aPawn, class ASidheRigelPlayerController* controller);
	// Setter to set camera from another scripts
	FORCEINLINE void SetCamera(class ASidheRigelCamera* NewCamera) { SRCamera = NewCamera; }
protected:
	// Reference to our camera
	UPROPERTY(Replicated)
	class ASidheRigelCamera* SRCamera;
};


