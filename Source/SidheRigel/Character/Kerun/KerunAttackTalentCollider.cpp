// Fill out your copyright notice in the Description page of Project Settings.


#include "KerunAttackTalentCollider.h"

#include "Components/SphereComponent.h"

#include "SidheRigel/Interface/Team.h"
#include "SidheRigel/Interface/Damagable.h"

// Sets default values
AKerunAttackTalentCollider::AKerunAttackTalentCollider()
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

	CollisionComponent->OnComponentBeginOverlap.AddDynamic(this, &AKerunAttackTalentCollider::OnOverlap);


}

// Called when the game starts or when spawned
void AKerunAttackTalentCollider::BeginPlay()
{
	Super::BeginPlay();

	FTimerHandle R1TalentColliderTimer;
	GetWorldTimerManager().SetTimer(R1TalentColliderTimer,
		FTimerDelegate::CreateLambda([=]()
			{
				Destroy();
			}
	), ColliderDuration, false);	
}

// Called every frame
void AKerunAttackTalentCollider::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

void AKerunAttackTalentCollider::OnOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{
	if (ITeam* Enemy = Cast<ITeam>(OtherActor))
	{
		if (Enemy->GetTeam() != Cast<ITeam>(colliderOwner)->GetTeam())
		{
			if (IDamagable* DamagableActor = Cast<IDamagable>(OtherActor))
			{
				DamagableActor->TakeDamage(damage, colliderOwner);
			}
		}
	}
}

