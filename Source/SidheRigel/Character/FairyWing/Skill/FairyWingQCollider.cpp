#include "FairyWingQCollider.h"
#include "GameFramework/ProjectileMovementComponent.h"
#include "Components/SphereComponent.h"
#include "kismet/GameplayStatics.h"

#include "SidheRigel/Interface/Damagable.h"
#include "SidheRigel/Interface/Team.h"
#include "SidheRigel/Interface/CCable.h"
#include "SidheRigel/SidheRigelCharacter.h"
#include "../FairyWingCharacter.h"
#include "../../../Minion/Minion.h"

// Sets default values
AFairyWingQCollider::AFairyWingQCollider()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	if (!RootComponent)
	{
		RootComponent = CreateDefaultSubobject<USceneComponent>(TEXT("ColliderSceneComponent"));
	}
	if (!CollisionComponent)
	{
		// Use a sphere as a simple collision representation.
		CollisionComponent = CreateDefaultSubobject<USphereComponent>(TEXT("SphereComponent"));
		// Set the sphere's collision radius.
		CollisionComponent->InitSphereRadius(15.0f);
		// Set the root component to be the collision component.
		RootComponent = CollisionComponent;
	}
	if (!ColliderMesh)
	{
		ColliderMesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("ColliderMesh"));
		ColliderMesh->SetupAttachment(CollisionComponent);

		static ConstructorHelpers::FObjectFinder<UStaticMesh>Mesh(TEXT("/Game/Heros/FairyWing/Skill/SM_FairyWingQCollider"));
		if (Mesh.Succeeded())
		{
			ColliderMesh->SetStaticMesh(Mesh.Object);
		}
	}
}

// Called when the game starts or when spawned
void AFairyWingQCollider::BeginPlay()
{
	Super::BeginPlay();

	UGameplayStatics::PlaySoundAtLocation(this, SpawnSound, this->GetActorLocation());

	TSet<AActor*> overlapActors;
	CollisionComponent->GetOverlappingActors(overlapActors);
	for (auto& OtherActor : overlapActors)
	{
		if (AFairyWingCharacter* ColliderOwner = Cast<AFairyWingCharacter>(colliderOwner))
		{
			if (ColliderOwner->IsSelectedTalent[1][1])
			{
				if (ITeam* team = Cast<ITeam>(OtherActor))
				{
					if (team->GetTeam() != Cast<ITeam>(colliderOwner)->GetTeam())
					{
						if (ICCable* target = Cast<ICCable>(OtherActor))
						{
							target->Stop(0.5f);
						}
					}
				}
			}			
		}		
	}	

	FTimerHandle QColliderDestroyTimer;
	GetWorldTimerManager().SetTimer(QColliderDestroyTimer,
		FTimerDelegate::CreateLambda([=]()
			{
				Destroy();
			}
	), duration, false);
}

// Called every frame
void AFairyWingQCollider::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	elapsedTime += DeltaTime;

	if (elapsedTime > 0.2f)
	{
		TSet<AActor*> overlapActors;
		CollisionComponent->GetOverlappingActors(overlapActors);

		for (auto& OtherActor : overlapActors)
		{
			if (ITeam* team = Cast<ITeam>(OtherActor))
			{
				if (team->GetTeam() != Cast<ITeam>(colliderOwner)->GetTeam())
				{
					if (IDamagable* target = Cast<IDamagable>(OtherActor))
					{
						target->TakeDamage(damage, colliderOwner);
					}

					if (ASidheRigelCharacter* target = Cast<ASidheRigelCharacter>(OtherActor))
					{
						if (target->isBombMarkAlreadyHit)
						{
							if (IDamagable* enemy = Cast<IDamagable>(OtherActor))
							{
								enemy->TakeDamage(damage, colliderOwner);
								target->isBombMarkAlreadyHit = false;
							}
						}

						if (target->isWSkillAlreadyHit)
						{
							if (ICCable* CCtarget = Cast<ICCable>(OtherActor))
							{
								CCtarget->Stun(1.f);
								target->isWSkillAlreadyHit = false;
							}
						}
					}
				}
				else if (team->GetTeam() == Cast<ITeam>(colliderOwner)->GetTeam())
				{
					if (ASidheRigelCharacter* target = Cast<ASidheRigelCharacter>(OtherActor))
					{
						target->AddSpeed(FString(TEXT("FairyWing_Q_SpeedBuf")), increaseSpeed, increaseSpeedTime);
						target->AddAttackSpeed(FString(TEXT("FairyWing_Q_AttackSpeedBuf")), increaseAttackSpeed);
					}

					if (IDamagable* target = Cast<IDamagable>(OtherActor))
					{
						target->RestoreHP(restoreHPValue);
					}
				}
			}
		}

		elapsedTime = 0;
	}
}