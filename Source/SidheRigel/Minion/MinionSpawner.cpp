// Fill out your copyright notice in the Description page of Project Settings.


#include "MinionSpawner.h"

#include "SidheRigel/Minion/Minion.h"

// Sets default values
AMinionSpawner::AMinionSpawner()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	static ConstructorHelpers::FObjectFinder<UBlueprint> BPMinion(TEXT("/Game/Minion/BP_Minion"));
	if (BPMinion.Object)
	{
		minionClass = (UClass*)BPMinion.Object->GeneratedClass;
	}
}

// Called when the game starts or when spawned
void AMinionSpawner::BeginPlay()
{
	Super::BeginPlay();
}

// Called every frame
void AMinionSpawner::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	DebugTimer += DeltaTime;

	if (DebugTimer > 5.f)
	{
		SpawnMinion();
		DebugTimer = 0;
	}
}

void AMinionSpawner::SpawnMinion()
{
	if (minionClass)
	{
		FVector SpawnLocation = GetActorLocation();
		FRotator SpawnRotation = GetActorRotation();

		UWorld* World = GetWorld();
		if (World)
		{
			FActorSpawnParameters SpawnParams;
			FTransform SpawnTransform;
			SpawnTransform.SetLocation(SpawnLocation);
			SpawnTransform.SetRotation(SpawnRotation.Quaternion());
			SpawnParams.Owner = this;
			SpawnParams.SpawnCollisionHandlingOverride = ESpawnActorCollisionHandlingMethod::AlwaysSpawn;

			AMinion* minion = World->SpawnActorDeferred<AMinion>(minionClass, SpawnTransform);
			if (minion)
			{
				minion->SetTeam(GetTeam());
			}

			minion->FinishSpawning(SpawnTransform);
		}
	}
}

E_Team AMinionSpawner::GetTeam()
{
	return team;
}

