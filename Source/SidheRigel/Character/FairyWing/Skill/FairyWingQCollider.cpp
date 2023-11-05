#include "FairyWingQCollider.h"
#include "GameFramework/ProjectileMovementComponent.h"
#include "Components/SphereComponent.h"
#include "SidheRigel/Interface/Damagable.h"
#include "SidheRigel/Interface/Team.h"

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
			}
			else if(team->GetTeam() == Cast<ITeam>(colliderOwner)->GetTeam())
			{
				// 같은 팀이면 이동속도 증가

				if (IDamagable* target = Cast<IDamagable>(OtherActor))
				{
					target->RestoreHP(restoreHPValue);
				}
			}
		}
	}
}