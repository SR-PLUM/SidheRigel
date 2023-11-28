// Fill out your copyright notice in the Description page of Project Settings.


#include "MinionSpawner.h"
#include "Net/UnrealNetwork.h"

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

	static ConstructorHelpers::FObjectFinder<USkeletalMesh> WMMesh(TEXT("/Game/Minion/Minion_Lane_Melee_Dawn"));
	if (WMMesh.Succeeded())
	{
		whiteMeleeMinionMesh = WMMesh.Object;
	}
	static ConstructorHelpers::FObjectFinder<USkeletalMesh> WRMesh(TEXT("/Game/Minion/Minion_Lane_Ranged_Dawn"));
	if (WRMesh.Succeeded())
	{
		whiteRangeMinionMesh = WRMesh.Object;
	}
	static ConstructorHelpers::FObjectFinder<USkeletalMesh> BMMesh(TEXT("/Game/Minion/Minion_Lane_Melee_Dusk")); 
	if (BMMesh.Succeeded())
	{
		blackMeleeMinionMesh = BMMesh.Object;
	}
	static ConstructorHelpers::FObjectFinder<USkeletalMesh> BRMesh(TEXT("/Game/Minion/Minion_Lane_Ranged_Dusk"));
	if (BRMesh.Succeeded())
	{
		blackRangeMinionMesh = BRMesh.Object;
	}

	bReplicates = true;
}

void AMinionSpawner::GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const
{
	Super::GetLifetimeReplicatedProps(OutLifetimeProps);

	DOREPLIFETIME(AMinionSpawner, team);
}

// Called when the game starts or when spawned
void AMinionSpawner::BeginPlay()
{
	Super::BeginPlay();

	Server_SpawnMinionWave();
}

// Called every frame
void AMinionSpawner::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	DebugTimer += DeltaTime;

	if (DebugTimer > 40.f)
	{
		Server_SpawnMinionWave();
		DebugTimer = 0;
	}
}

void AMinionSpawner::Server_SpawnMinionWave_Implementation()
{
	if (HasAuthority())
	{
		for (int i = 0; i < 6; i++)
		{
			FTimerHandle minionGenerateTimer;
			if (i < 3)
			{
				GetWorldTimerManager().SetTimer(minionGenerateTimer, this, &AMinionSpawner::Server_SpawnMinion, i + 0.1, false);
			}
			else
			{
				GetWorldTimerManager().SetTimer(minionGenerateTimer, this, &AMinionSpawner::Server_SpawnRangeMinion, i + 0.1, false);
			}
		}
	}
}

void AMinionSpawner::Server_SpawnMinion()
{
	if(!HasAuthority())
		UE_LOG(LogTemp,Warning,TEXT("NotServer In ServerSpawnMinion"))
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
				if (GetTeam() == E_Team::Blue)
				{
					minion->GetMesh()->SetSkeletalMesh(whiteMeleeMinionMesh);
				}
				else
				{
					minion->GetMesh()->SetSkeletalMesh(blackMeleeMinionMesh);
				}

				minion->ownerSpawner = this;
			}
			minion->FinishSpawning(SpawnTransform);
		}
	}
}

void AMinionSpawner::Server_SpawnRangeMinion()
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
				if (GetTeam() == E_Team::Blue)
				{
					UE_LOG(LogTemp, Warning, TEXT("MinionSpawner :: Team is Blue"));
					rangeMinion->GetMesh()->SetSkeletalMesh(whiteRangeMinionMesh);
				}
				else
				{
					UE_LOG(LogTemp, Warning, TEXT("MinionSpawner :: Team is Red"));
					rangeMinion->GetMesh()->SetSkeletalMesh(blackRangeMinionMesh);
				}

				rangeMinion->ownerSpawner = this;
			}

			rangeMinion->FinishSpawning(SpawnTransform);
		}
	}
}



E_Team AMinionSpawner::GetTeam()
{
	return team;
}

