// Fill out your copyright notice in the Description page of Project Settings.


#include "BlackWizardECollider.h"
#include "GameFramework/ProjectileMovementComponent.h"
#include "Components/SphereComponent.h"
#include "SidheRigel/Interface/CCable.h"
#include "SidheRigel/Interface/Damagable.h"

// Sets default values
ABlackWizardECollider::ABlackWizardECollider()
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

		static ConstructorHelpers::FObjectFinder<UStaticMesh>Mesh(TEXT("/Game/Heros/BlackWizard/Skill/SM_BlackWizardECollider"));
		if (Mesh.Succeeded())
		{
			ColliderMesh->SetStaticMesh(Mesh.Object);
		}
	}
}

// Called when the game starts or when spawned
void ABlackWizardECollider::BeginPlay()
{
	Super::BeginPlay();

	CollisionComponent->OnComponentBeginOverlap.AddDynamic(this, &ABlackWizardECollider::OnColliderOverlap);

	FTimerHandle EColliderDestroyTimer;
	GetWorldTimerManager().SetTimer(EColliderDestroyTimer,
		FTimerDelegate::CreateLambda([=]()
			{
				Destroy();
			}

	), duration, false);
}

// Called every frame
void ABlackWizardECollider::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

void ABlackWizardECollider::OnColliderOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{
	if (OtherActor)
	{
		if (IDamagable* test = Cast<IDamagable>(OtherActor))
		{
			test->TakeDamage(10.f, colliderOwner);
		}			

		if (ICCable* CC = Cast<ICCable>(OtherActor))
		{
			CC->Stop(1.0f);
		}			
	}
}

