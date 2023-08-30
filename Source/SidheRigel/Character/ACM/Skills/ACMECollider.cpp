// Fill out your copyright notice in the Description page of Project Settings.


#include "SidheRigel/Character/ACM/Skills/ACMECollider.h"
#include "SidheRigel/Interface/Damagable.h"
#include "SidheRigel/Interface/CCable.h"
#include "SidheRigel/SidheRigelCharacter.h"

// Sets default values
AACMECollider::AACMECollider()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	if (!RootComponent)
	{
		RootComponent = CreateDefaultSubobject<USceneComponent>(TEXT("ColliderSceneComponent"));
	}
	if (!MeshComponent)
	{
		MeshComponent = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("MeshComponent"));
		MeshComponent->SetupAttachment(RootComponent);

		static ConstructorHelpers::FObjectFinder<UStaticMesh>Mesh(TEXT("/Game/Heros/ACM/Skills/SM_ACMECollider"));
		if (Mesh.Succeeded())
		{
			MeshComponent->SetStaticMesh(Mesh.Object);
		}
	}

	InitStatus();
}

// Called when the game starts or when spawned
void AACMECollider::BeginPlay()
{
	Super::BeginPlay();
	
	MeshComponent->OnComponentBeginOverlap.AddDynamic(this, &AACMECollider::OnColliderOverlap);
}

// Called every frame
void AACMECollider::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

void AACMECollider::OnColliderOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{
	if (OtherActor)
	{
		IDamagable* DamagableTarget = Cast<IDamagable>(OtherActor);
		if (DamagableTarget && (OtherActor != ColliderOwner))
		{
			DamagableTarget->TakeDamage(CurrentDamage, ColliderOwner);
		}

		ICCable* CCableTarget = Cast<ICCable>(OtherActor);
		if (CCableTarget && (OtherActor != ColliderOwner))
		{
			CCableTarget->Stun(StunTime);
			if (IsUpgraded)
			{
				CCableTarget->Airborne(AirBoneTime);
			}
		}
	}
}

void AACMECollider::InitStatus()
{
	if (IsUpgraded)
	{
		CurrentDamage = UpgradedDamage;
	}
	else
	{
		CurrentDamage = Damage;
	}
}
