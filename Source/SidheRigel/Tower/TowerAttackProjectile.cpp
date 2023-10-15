

#include "TowerAttackProjectile.h"
#include "GameFramework/ProjectileMovementComponent.h"

#include "SidheRigel/Interface/Damagable.h"

// Sets default values
ATowerAttackProjectile::ATowerAttackProjectile()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	if (!ProjectileMesh)
	{
		ProjectileMesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("ProjectileMesh"));

		static ConstructorHelpers::FObjectFinder<UStaticMesh>Mesh(TEXT("/Game/Tower/MaterialSphere"));
		if (Mesh.Succeeded())
		{
			ProjectileMesh->SetStaticMesh(Mesh.Object);
		}

		RootComponent = ProjectileMesh;

		CreateDefaultSubobject<USceneComponent>(TEXT("ProjectileSceneComponent"))->SetupAttachment(RootComponent);
	}
	if (!ProjectileMovementComponent)
	{
		ProjectileMovementComponent = CreateDefaultSubobject<UProjectileMovementComponent>(TEXT("ProjectileMovementComponent"));
		ProjectileMovementComponent->SetUpdatedComponent(ProjectileMesh);
		ProjectileMovementComponent->InitialSpeed = 10.f;
		ProjectileMovementComponent->MaxSpeed = 0.f;
		ProjectileMovementComponent->bRotationFollowsVelocity = true;
		ProjectileMovementComponent->bShouldBounce = false;
		ProjectileMovementComponent->Bounciness = 0.f;
		ProjectileMovementComponent->ProjectileGravityScale = 0.0f;
	}
}

// Called when the game starts or when spawned
void ATowerAttackProjectile::BeginPlay()
{
	Super::BeginPlay();
	
}

// Called every frame
void ATowerAttackProjectile::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	if (target)
	{
		FVector Forward = (target->GetActorLocation() - GetActorLocation()).GetSafeNormal();
		ProjectileMovementComponent->Velocity = (Forward * speed);
		if ((GetDistanceTo(target)) < 100.f)
		{
			if (IDamagable* damagableTarget = Cast<IDamagable>(target))
			{
				damagableTarget->TakeDamage(damage, projectileOwner);
			}

			target = nullptr;
			Destroy();
		}
	}
	else
	{
		Destroy();
	}
}

