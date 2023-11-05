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

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	UPROPERTY(EditAnywhere)
		TArray<USkeletalMesh*> meshArray;

	UPROPERTY(EditAnywhere)
		TEnumAsByte<E_Team> team = E_Team::Red;

	UFUNCTION()
		void SpawnMinion();

	UPROPERTY()
		float DebugTimer = 0;

	UFUNCTION()
		virtual E_Team GetTeam() override;
};
