// Fill out your copyright notice in the Description page of Project Settings.


#include "MinionSpawner.h"

#include "SidheRigel/Minion/Minion.h"
#include "SidheRigel/Minion/RangeMinion.h"

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

	static ConstructorHelpers::FObjectFinder<UBlueprint> BPRangeMinion(TEXT("/Game/Minion/BP_RangeMinion"));
	if (BPRangeMinion.Object)
	{
		rangeMinionClass = (UClass*)BPRangeMinion.Object->GeneratedClass;
	}
}

// Called when the game starts or when spawned
void AMinionSpawner::BeginPlay()
{
	Super::BeginPlay();

	SpawnMinionWave();
}

// Called every frame
void AMinionSpawner::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	DebugTimer += DeltaTime;

	if (DebugTimer > 40.f)
	{
		SpawnMinionWave();
		DebugTimer = 0;
	}
}

void AMinionSpawner::SpawnMinionWave()
{
	for (int i = 0; i < 6; i++)
	{
		FTimerHandle minionGenerateTimer;
		if (i < 3)
		{
			GetWorldTimerManager().SetTimer(minionGenerateTimer, this, &AMinionSpawner::SpawnMinion, i+0.1, false);
		}
		else
		{
			GetWorldTimerManager().SetTimer(minionGenerateTimer, this, &AMinionSpawner::SpawnRangeMinion, i+0.1, false);
		}
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
				minion->GetMesh()->SetSkeletalMesh(meshArray[0]);
			}

			minion->FinishSpawning(SpawnTransform);
		}
	}
}

void AMinionSpawner::SpawnRangeMinion()
{
	if (rangeMinionClass)
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

			ARangeMinion* rangeMinion = World->SpawnActorDeferred<ARangeMinion>(rangeMinionClass, SpawnTransform);
			if (rangeMinion)
			{
				rangeMinion->SetTeam(GetTeam());
				rangeMinion->GetMesh()->SetSkeletalMesh(meshArray[1]);
			}

			rangeMinion->FinishSpawning(SpawnTransform);
		}
	}
}



E_Team AMinionSpawner::GetTeam()
{
	return team;
}

