// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"

#include "SidheRigel/Interface/Team.h"

#include "MinionSpawner.generated.h"

UCLASS()
class SIDHERIGEL_API AMinionSpawner : public AActor, public ITeam
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	AMinionSpawner();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

	UPROPERTY(EditDefaultsOnly)
		TSubclassOf<class AMinion> minionClass;
	UPROPERTY(EditDefaultsOnly)
		TSubclassOf<class ARangeMinion> rangeMinionClass;
	UPROPERTY()
		class USkeletalMesh* whiteMeleeMinionMesh;
	UPROPERTY()
		class USkeletalMesh* whiteRangeMinionMesh;
	UPROPERTY()
		class USkeletalMesh* blackMeleeMinionMesh;
	UPROPERTY()
		class USkeletalMesh* blackRangeMinionMesh;

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	UPROPERTY(EditAnywhere, replicated)
		TEnumAsByte<E_Team> team;

	UFUNCTION(reliable, server)
		void Server_SpawnMinionWave();
	UFUNCTION()
		void Server_SpawnMinion();
	UFUNCTION()
		void Server_SpawnRangeMinion();

	UPROPERTY()
		float DebugTimer = 0;

	UFUNCTION()
		virtual E_Team GetTeam() override;
};
