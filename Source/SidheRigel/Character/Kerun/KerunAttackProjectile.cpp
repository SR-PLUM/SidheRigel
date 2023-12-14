// Fill out your copyright notice in the Description page of Project Settings.


#include "KerunAttackProjectile.h"
#include "GameFramework/ProjectileMovementComponent.h"
#include "Components/SphereComponent.h"
#include "../../Interface/Damagable.h"
#include "Math/UnrealMathUtility.h"
#include "SidheRigel/Character/Kerun/KerunCharacter.h"
#include "SidheRigel/Character/Kerun/Skills/KerunQSkill.h"
#include "KerunAttackTalentCollider.h"

// Sets default values
AKerunAttackProjectile::AKerunAttackProjectile()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	static ConstructorHelpers::FObjectFinder<UClass>colliderRef(TEXT("Blueprint'/Game/Heros/Kerun/BP_KerunAttackTalentCollider.BP_KerunAttackTalentCollider_C'"));
	if (colliderRef.Object)
	{
		colliderClass = (UClass*)colliderRef.Object;
	}
}

// Called when the game starts or when spawned
void AKerunAttackProjectile::BeginPlay()
{
	Super::BeginPlay();
	
}

// Called every frame
void AKerunAttackProjectile::Tick(float DeltaTime)
{
	AActor::Tick(DeltaTime);

	if (Target)
	{
		FVector Forward = (Target->GetActorLocation() - GetActorLocation()).GetSafeNormal();
		ProjectileMovementComponent->Velocity = (Forward * speed);
		if ((this->GetDistanceTo(Target)) < 100.f)
		{
			float totalAttackDamage = AttackDamage;

			AKerunCharacter* character = Cast<AKerunCharacter>(projectileOwner);

			UKerunQSkill* QSkillRef = Cast<UKerunQSkill>(character->skills[E_SkillState::Q_Ready]);

			if (QSkillRef->IsWorking)
			{
				character->UseQParticle(Target);

				QSkillRef->AttackCount += 1;
				UE_LOG(LogTemp, Warning, TEXT("AttackCount :: %d"), QSkillRef->AttackCount)
			}

			QSkillRef->ApplyTalentWhenFullComboHits(Target);

			if (character->IsSelectedTalent[5][0] && QSkillRef->IsWorking)
			{
				UE_LOG(LogTemp, Warning, TEXT("CRITICAL!"));
				totalAttackDamage *= criticalDamage;
			}
			else
			{
				if (FMath::RandRange(0, 1) <= criticalRate)
				{
					UE_LOG(LogTemp, Warning, TEXT("CRITICAL!"));
					totalAttackDamage *= criticalDamage;
				}
			}

			if (IDamagable* damagableTarget = Cast<IDamagable>(Target))
			{
				damagableTarget->TakeDamage(totalAttackDamage, projectileOwner);

				if (IsValid(character))
				{
					if (character->IsSelectedTalent[4][0])
					{
						float lifeSteal = character->KerunTalent40LifeStealAmount / 100.f;
						
						character->RestoreHP(lifeSteal);
					}
				}

				if (character->IsSelectedTalent[6][0])
				{
					UWorld* world = character->GetWorld();
					if (world)
					{
						FActorSpawnParameters SpawnParams;
						FTransform SpawnTransform;
						SpawnTransform.SetLocation(GetActorLocation());
						SpawnTransform.SetRotation(GetActorRotation().Quaternion());
						SpawnParams.Owner = character;
						SpawnParams.SpawnCollisionHandlingOverride = ESpawnActorCollisionHandlingMethod::AlwaysSpawn;


						AKerunAttackTalentCollider* collider = world->SpawnActorDeferred<AKerunAttackTalentCollider>(colliderClass, SpawnTransform);
						if (collider)
						{
							collider->colliderOwner = character;
							collider->damage = totalAttackDamage;
						}
						collider->FinishSpawning(SpawnTransform);

					}
				}
			}

			Target = nullptr;

			PlayHitSound();
			Destroy();
		}
	}

}

void AKerunAttackProjectile::SetProjectileMesh()
{
}

void AKerunAttackProjectile::SetProjectileMovementComponent()
{
	ProjectileMovementComponent = CreateDefaultSubobject<UProjectileMovementComponent>(TEXT("ProjectileMovementComponent"));
	ProjectileMovementComponent->InitialSpeed = 5.f;
	ProjectileMovementComponent->MaxSpeed = 5.f;
	ProjectileMovementComponent->bRotationFollowsVelocity = true;
	ProjectileMovementComponent->bShouldBounce = false;
	ProjectileMovementComponent->Bounciness = 0.f;
	ProjectileMovementComponent->ProjectileGravityScale = 0.0f;
}

