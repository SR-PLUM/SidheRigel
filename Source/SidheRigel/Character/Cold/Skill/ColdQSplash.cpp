// Fill out your copyright notice in the Description page of Project Settings.


#include "ColdQSplash.h"

#include "Components/SphereComponent.h"

#include "SidheRigel/Interface/Team.h"
#include "SidheRigel/Interface/Damagable.h"

// Sets default values
AColdQSplash::AColdQSplash()
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
}

// Called when the game starts or when spawned
void AColdQSplash::BeginPlay()
{
	Super::BeginPlay();
	
	CollisionComponent->OnComponentBeginOverlap.AddDynamic(this, &AColdQSplash::OnOverlap);
	
	FTimerHandle QSpalshParticleTimer;
	GetWorldTimerManager().SetTimer(QSpalshParticleTimer,
		FTimerDelegate::CreateLambda([=]()
			{
				Destroy();
			}
	), particleDuration, false);
}

// Called every frame
void AColdQSplash::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	elapsedTime += DeltaTime;

	

}

void AColdQSplash::OnOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{
	if (elapsedTime > 0.1f)
		return;

	if (ITeam* Enemy = Cast<ITeam>(OtherActor))
	{
		if (Enemy->GetTeam() != Cast<ITeam>(projectileOwner)->GetTeam())
		{
			if (IDamagable* DamagableActor = Cast<IDamagable>(OtherActor))
			{
				DamagableActor->TakeDamage(damage, projectileOwner);
			}
		}
	}
}

