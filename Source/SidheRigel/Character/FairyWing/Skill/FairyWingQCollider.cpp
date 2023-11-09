#include "FairyWingQCollider.h"
#include "GameFramework/ProjectileMovementComponent.h"
#include "Components/SphereComponent.h"
#include "SidheRigel/Interface/Damagable.h"
#include "SidheRigel/Interface/Team.h"
#include "SidheRigel/Interface/CCable.h"

#include "SidheRigel/SidheRigelCharacter.h"
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

	CollisionComponent->OnComponentBeginOverlap.AddDynamic(this, &AFairyWingQCollider::OnColliderOverlap);

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

}

void AFairyWingQCollider::OnColliderOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{
	if (OtherActor)
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
						}
					}

					if (target->isWSkillAlreadyHit)
					{
						if (ICCable* CCtarget = Cast<ICCable>(OtherActor))
						{
							CCtarget->Stun(3.f);
						}
					}
				}

				if (ICCable* target = Cast<ICCable>(OtherActor))
				{
					//target->Blind(blindTime);
				}
			}
			else if(team->GetTeam() == Cast<ITeam>(colliderOwner)->GetTeam())
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
}