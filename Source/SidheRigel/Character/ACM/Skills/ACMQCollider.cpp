// Fill out your copyright notice in the Description page of Project Settings.


#include "ACMQCollider.h"
#include "SidheRigel/Interface/Damagable.h"
#include "SidheRigel/SidheRigelCharacter.h"

// Sets default values
AACMQCollider::AACMQCollider()
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

		static ConstructorHelpers::FObjectFinder<UStaticMesh>Mesh(TEXT("/Game/Heros/ACM/Skills/SM_ACMQCollider"));
		if (Mesh.Succeeded())
		{
			MeshComponent->SetStaticMesh(Mesh.Object);

			FVector Prev = MeshComponent->GetRelativeScale3D();
			Prev *= (CurrentRange, 1, 1);
			MeshComponent->SetRelativeScale3D(Prev);
		}
	}

	InitStatus();
}

// Called when the game starts or when spawned
void AACMQCollider::BeginPlay()
{
	Super::BeginPlay();
	
	MeshComponent->OnComponentBeginOverlap.AddDynamic(this, &AACMQCollider::OnColliderOverlap);
}

// Called every frame
void AACMQCollider::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

void AACMQCollider::OnColliderOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{
	if (OtherActor)
	{
		IDamagable* DamagableTarget = Cast<IDamagable>(OtherActor);
		if (DamagableTarget && (OtherActor != ColliderOwner))
		{
			DamagableTarget->TakeDamage(CurrentDamage, ColliderOwner);
		}

		if (IsUpgraded)
		{
			ASidheRigelCharacter* characterTarget = Cast<ASidheRigelCharacter>(OtherActor);
			if (characterTarget)
			{
				//characterTarget->AddDecreseDefencePercent("DebuffName", 0.1f, 1f);
			}
		}

	}
}

void AACMQCollider::InitStatus()
{
	if (IsUpgraded)
	{
		CurrentDamage = UpgradedDamage;
		CurrentRange = UpgradedRange;
	}
	else
	{
		CurrentDamage = Damage;
		CurrentRange = Range;
	}
}
