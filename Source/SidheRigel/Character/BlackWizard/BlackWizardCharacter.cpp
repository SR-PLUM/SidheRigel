#include "BlackWizardCharacter.h"
#include "Components/SphereComponent.h"
#include "SidheRigel/Character/BlackWizard/Skill/BlackWizardRCollider.h"
#include "SidheRigel/Character/BlackWizard/BlackWizardAttackProjectile.h"

// Sets default values
ABlackWizardCharacter::ABlackWizardCharacter()
{

	skillState = Null;

 	// Set this character to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	static ConstructorHelpers::FObjectFinder<UBlueprint> RCollider(TEXT("/Game/Heros/BlackWizard/Skill/BP_BlackWizardRCollider"));
	if (RCollider.Object)
	{
		RColliderClass = (UClass*)RCollider.Object->GeneratedClass;
	}

	InitAttackProjectile();
}

// Called when the game starts or when spawned
void ABlackWizardCharacter::BeginPlay()
{
	Super::BeginPlay();
	
}

// Called every frame
void ABlackWizardCharacter::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

// Called to bind functionality to input
void ABlackWizardCharacter::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);

}

void ABlackWizardCharacter::InitProperty()
{
	range.Add("Debug", 150.f);
	attackDamage.Add("Debug", 5.f);
	attackSpeed.Add("Debug", 1.f);
	criticalRate.Add("Debug", 50);
	criticalDamage.Add("Debug", 50);
	MaxHP.Add("Debug", 100.f);
	generateHealthPoint.Add("Debug", 0.2f);
	lifeSteal.Add("Debug", 5.f);
	protectPower.Add("Debug", 20);

	currentHP = GetMaxHP();
}

void ABlackWizardCharacter::InitAttackProjectile()
{
	static ConstructorHelpers::FObjectFinder<UBlueprint> Projectile(TEXT("/Game/Heros/BlackWizard/BP_BlackWizardAttackProjectile"));
	if (Projectile.Object)
	{
		attackProjectileClass = (UClass*)Projectile.Object->GeneratedClass;
	}
}

void ABlackWizardCharacter::SpawnAttackProjectile()
{
	if (attackProjectileClass)
	{
		FVector MuzzleLocation = GetActorLocation();
		FRotator MuzzleRotation = GetActorRotation();

		UWorld* World = GetWorld();
		if (World)
		{
			FActorSpawnParameters SpawnParams;
			SpawnParams.Owner = this;
			SpawnParams.Instigator = GetInstigator();

			// Spawn the projectile at the muzzle.
			ABlackWizardAttackProjectile* Projectile = World->SpawnActor<ABlackWizardAttackProjectile>(attackProjectileClass, MuzzleLocation, MuzzleRotation, SpawnParams);
			if (Projectile)
			{
				InitProjectileProperty(Projectile);
			}
		}
	}
}

void ABlackWizardCharacter::SkillOne()
{
	UE_LOG(LogTemp, Warning, TEXT("BlackWizard Ready Q"));

	skillState = Q_Ready;
}

void ABlackWizardCharacter::SkillTwo()
{
	UE_LOG(LogTemp, Warning, TEXT("BlackWizard Ready W"));

	skillState = W_Ready;
}

void ABlackWizardCharacter::SkillThree()
{
	UE_LOG(LogTemp, Warning, TEXT("BlackWizard Ready E"));

	skillState = E_Ready;
}

void ABlackWizardCharacter::SkillFour()
{
	UE_LOG(LogTemp, Warning, TEXT("BlackWizard Ready R"));

	skillState = R_Ready;
}

void ABlackWizardCharacter::SkillCancel()
{
	UE_LOG(LogTemp, Warning, TEXT("BlackWizard SkillCancel"));

	skillState = Null;
}

void ABlackWizardCharacter::UseSkill(FHitResult HitResult)
{
	switch (skillState)
	{
	case Null:
		UE_LOG(LogTemp, Warning, TEXT("BlackWizard SkillState is Null"));
		break;
	case Q_Ready:
		UE_LOG(LogTemp, Warning, TEXT("BlackWizard use Q"));
		skillState = Null;
		break;
	case W_Ready:
		UE_LOG(LogTemp, Warning, TEXT("BlackWizard use W"));
		skillState = Null;
		break;
	case E_Ready:
		UE_LOG(LogTemp, Warning, TEXT("BlackWizard use E"));
		skillState = Null;
		break;
	case R_Ready:
		UE_LOG(LogTemp, Warning, TEXT("BlackWizard use R"));

		if (RColliderClass)
		{
			FVector PawnToTarget = (HitResult.Location - GetActorLocation()).GetSafeNormal();
			PawnToTarget *= FVector(1, 1, 0);
			FVector MuzzleLocation = (GetActorLocation() + PawnToTarget);
			FRotator MuzzleRotation = PawnToTarget.Rotation();
			UWorld* World = GetWorld();
			if (World)
			{
				FActorSpawnParameters SpawnParams;
				FTransform SpawnTransform;
				SpawnTransform.SetLocation(MuzzleLocation);
				SpawnTransform.SetRotation(MuzzleRotation.Quaternion());
				SpawnParams.Owner = this;
				SpawnParams.SpawnCollisionHandlingOverride = ESpawnActorCollisionHandlingMethod::AlwaysSpawn;
				//SpawnParams.Instigator = GetInstigator();

				// Spawn the projectile at the muzzle.
				ABlackWizardRCollider* Collider = World->SpawnActorDeferred<ABlackWizardRCollider>(RColliderClass, SpawnTransform);
				if (Collider)
				{
					// Set the projectile's initial trajectory.
					Collider->colliderOwner = this;

					FTimerHandle RColliderDestroyTimer;
					GetWorldTimerManager().SetTimer(RColliderDestroyTimer,
						FTimerDelegate::CreateLambda([=]()
							{
								Collider->Destroy();
							}

					), 1.0f, false);					

					Collider->FinishSpawning(SpawnTransform);

					Collider->CollisionComponent->SetGenerateOverlapEvents(false);
				}
			}
		}

		skillState = Null;
		break;
	default:
		break;
	}
}