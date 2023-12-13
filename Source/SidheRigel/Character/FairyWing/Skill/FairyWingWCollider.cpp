#include "FairyWingWCollider.h"
#include "GameFramework/ProjectileMovementComponent.h"
#include "Components/SphereComponent.h"
#include "kismet/GameplayStatics.h"

#include "SidheRigel/Interface/Damagable.h"
#include "SidheRigel/Interface/CCable.h"
#include "SidheRigel/Interface/Team.h"
#include "SidheRigel/SidheRigelCharacter.h"
#include "../../../Minion/Minion.h"

// Sets default values
AFairyWingWCollider::AFairyWingWCollider()
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

		static ConstructorHelpers::FObjectFinder<UStaticMesh>Mesh(TEXT("/Game/Heros/FairyWing/Skill/SM_FairyWingWCollider"));
		if (Mesh.Succeeded())
		{
			ColliderMesh->SetStaticMesh(Mesh.Object);
		}
	}
}

// Called when the game starts or when spawned
void AFairyWingWCollider::BeginPlay()
{
	Super::BeginPlay();
	
	CollisionComponent->OnComponentBeginOverlap.AddDynamic(this, &AFairyWingWCollider::OnColliderOverlap);
	UGameplayStatics::PlaySoundAtLocation(this, SpawnSound, this->GetActorLocation());

	FTimerHandle WColliderDestroyTimer;
	GetWorldTimerManager().SetTimer(WColliderDestroyTimer,
		FTimerDelegate::CreateLambda([=]()
			{
				Destroy();
			}
	), duration, false);
}

// Called every frame
void AFairyWingWCollider::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

void AFairyWingWCollider::OnColliderOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{
	if (OtherActor)
	{
		if (ITeam* team = Cast<ITeam>(OtherActor))
		{
			if (team->GetTeam() != Cast<ITeam>(colliderOwner)->GetTeam())
			{
				IDamagable* target = Cast<IDamagable>(OtherActor);
				if (target)
					target->TakeDamage(damage, colliderOwner);

				if (ASidheRigelCharacter* markTarget = Cast<ASidheRigelCharacter>(OtherActor))
				{
					if (ASidheRigelCharacter* owner = Cast<ASidheRigelCharacter>(colliderOwner))
					{
						if (owner->IsSelectedTalent[2][0])
						{
							owner->RestoreHP(80.f);
						}

						if (owner->IsSelectedTalent[2][1])
						{
							markTarget->isWSkillAlreadyHit = true;
							FTimerHandle markDestroyTimer;
							GetWorldTimerManager().SetTimer(markDestroyTimer,
								FTimerDelegate::CreateLambda([=]()
									{
										markTarget->isWSkillAlreadyHit = false;
									}
							), 5.f, false);
						}

						if (markTarget->isBombMarkAlreadyHit)
						{
							if (IDamagable* enemy = Cast<IDamagable>(OtherActor))
							{
								enemy->TakeDamage(damage, colliderOwner);
							}
						}

						if (owner->IsSelectedTalent[4][0])
						{
							markTarget->AddAttackDamage("FariyWing_W", -20.f);

							FTimerHandle sightRestoreTimer;
							GetWorldTimerManager().SetTimer(sightRestoreTimer,
								FTimerDelegate::CreateLambda([=]()
									{
										markTarget->RemoveAttackDamage("FariyWing_W");
									}
							), 2.f, false);
						}
					}
				}

				ICCable* CC = Cast<ICCable>(OtherActor);
				if (CC)
					CC->Slow(1.0f, slowValue, "FairyWing_W");
			}			
		}		
	}
}