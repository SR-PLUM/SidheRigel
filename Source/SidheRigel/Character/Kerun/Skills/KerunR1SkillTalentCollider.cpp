// Fill out your copyright notice in the Description page of Project Settings.


#include "KerunR1SkillTalentCollider.h"

#include "Components/SphereComponent.h"
#include "kismet/GameplayStatics.h"

#include "SidheRigel/Interface/Team.h"
#include "SidheRigel/Interface/Damagable.h"
#include "SidheRigel/Interface/CCable.h"
#include "SidheRigel/Interface/Movable.h"

// Sets default values
AKerunR1SkillTalentCollider::AKerunR1SkillTalentCollider()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	if (!CollisionComponent)
	{
		// Use a sphere as a simple collision representation.
		CollisionComponent = CreateDefaultSubobject<USphereComponent>(TEXT("SphereComponent"));
		// Set the sphere's collision radius.
		CollisionComponent->InitSphereRadius(100.f);
		// Set the root component to be the collision component.
		RootComponent = CollisionComponent;
	}

	CollisionComponent->OnComponentBeginOverlap.AddDynamic(this, &AKerunR1SkillTalentCollider::OnOverlap);

}

// Called when the game starts or when spawned
void AKerunR1SkillTalentCollider::BeginPlay()
{
	Super::BeginPlay();
	UGameplayStatics::PlaySoundAtLocation(this, SpawnSound, this->GetActorLocation());
	
	FTimerHandle R1TalentColliderTimer;
	GetWorldTimerManager().SetTimer(R1TalentColliderTimer,
		FTimerDelegate::CreateLambda([=]()
			{
				Destroy();
			}
	), ColliderDuration, false);
}

// Called every frame
void AKerunR1SkillTalentCollider::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

void AKerunR1SkillTalentCollider::OnOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{
	if (ITeam* Enemy = Cast<ITeam>(OtherActor))
	{
		if (Enemy->GetTeam() != Cast<ITeam>(colliderOwner)->GetTeam())
		{
			if (ICCable* CCableActor = Cast<ICCable>(OtherActor))
			{
				CCableActor->Slow(SlowDuration, SlowAmount, "KerunR1SkillTalent");
			}

			if (IMovable* MoveActor = Cast<IMovable>(OtherActor))
			{
				if (colliderOwner)
				{
					FVector moveDirection = (OtherActor->GetActorLocation() - colliderOwner->GetActorLocation());

					MoveActor->MoveVector(moveDirection, ColliderForce);
				}
			}
		}
	}
}

