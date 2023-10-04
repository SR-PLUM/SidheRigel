// Fill out your copyright notice in the Description page of Project Settings.


#include "BlackWizardRCollider.h"
#include "GameFramework/ProjectileMovementComponent.h"
#include "Components/SphereComponent.h"
#include "SidheRigel/Interface/Damagable.h"
#include "SidheRigel/Interface/Movable.h"

// Sets default values
ABlackWizardRCollider::ABlackWizardRCollider()
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
		CollisionComponent->InitSphereRadius(50.0f);
		// Set the root component to be the collision component.
		RootComponent = CollisionComponent;
	}
	if (!ColliderMesh)
	{
		ColliderMesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("ColliderMesh"));
		ColliderMesh->SetupAttachment(CollisionComponent);

		static ConstructorHelpers::FObjectFinder<UStaticMesh>Mesh(TEXT("/Game/Heros/BlackWizard/Skill/SM_BlackWizardRCollider"));
		if (Mesh.Succeeded())
		{
			ColliderMesh->SetStaticMesh(Mesh.Object);
		}
	}
}

// Called when the game starts or when spawned
void ABlackWizardRCollider::BeginPlay()
{
	Super::BeginPlay();

	ColliderMesh->OnComponentBeginOverlap.AddDynamic(this, &ABlackWizardRCollider::OnColliderOverlap);

	FTimerHandle RColliderDestroyTimer;
	GetWorldTimerManager().SetTimer(RColliderDestroyTimer,
		FTimerDelegate::CreateLambda([=]()
			{
				Destroy();
			}

	), duration, false);
}

// Called every frame
void ABlackWizardRCollider::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

void ABlackWizardRCollider::OnColliderOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{
	if (OtherActor)
	{
		if (IDamagable* DamagableTarget = Cast<IDamagable>(OtherActor))
		{
			DamagableTarget->TakeDamage(damage, colliderOwner);
		}		

		if (IMovable* MovableTarget = Cast<IMovable>(OtherActor))
		{
			if (OtherActor != colliderOwner)
			{
				FVector moveDirection = (OtherActor->GetActorLocation() - colliderOwner->GetActorLocation()) * FVector(1, 1, 0);

				MovableTarget->MoveVector(moveDirection, force);
			}
		}
	}
}