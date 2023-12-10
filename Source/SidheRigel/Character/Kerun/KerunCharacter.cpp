// Fill out your copyright notice in the Description page of Project Settings.

#include "KerunCharacter.h"
#include "Kismet/GameplayStatics.h"

#include "../../Dummy/DummyProjectile.h"

#include "Skills/KerunQSkill.h"
#include "Skills/KerunWSkill.h"
#include "Skills/KerunR1Skill.h"
#include "Skills/KerunESkillTalentQuest.h"

#include "KerunAttackProjectile.h"
#include "KerunAnimInstance.h"

#include "GameFramework/ProjectileMovementComponent.h"
#include "GameFramework/CharacterMovementComponent.h"

#include "SidheRigel/SidheRigelPlayerController.h"

#include "SidheRigel/UI/InGameUI.h"
#include "SidheRigel/UI/CharacterStatus.h"
#include "SidheRigel/UI/StatSummary.h"

#include "Animation/AnimMontage.h"
#include "Particles/ParticleSystem.h"

AKerunCharacter::AKerunCharacter()
{
	InitAttackProjectile();

	//ProjectileMovementComponent = CreateDefaultSubobject<UProjectileMovementComponent>(TEXT("ProjectileMovementComponent"));

	InitKerunTalent();
}

void AKerunCharacter::BeginPlay()
{
	Super::BeginPlay();

	//AnimInstance = Cast<UKerunAnimInstance>(GetMesh()->GetAnimInstance());
	
	skills.Add({ E_SkillState::Q_Ready, NewObject<UKerunQSkill>() });
	if (skills[E_SkillState::Q_Ready] != nullptr)
	{
		skills[E_SkillState::Q_Ready]->SetSkillProperty(this, E_SkillState::Q_Ready);
	}
	
	skills.Add({ E_SkillState::W_Ready, NewObject<UKerunWSkill>() });
	if (skills[E_SkillState::W_Ready] != nullptr)
	{
		skills[E_SkillState::W_Ready]->SetSkillProperty(this, E_SkillState::W_Ready);
	}
	/*
	skills.Add({ E_SkillState::E_Ready, new ColdESkill });
	if (skills[E_SkillState::E_Ready] != nullptr)
	{
		skills[E_SkillState::E_Ready]->SetSkillProperty(this, E_SkillState::E_Ready);
	}*/
	skills.Add({ E_SkillState::R_Ready, NewObject<UKerunR1Skill>()});
	if (skills[E_SkillState::R_Ready] != nullptr)
	{
		skills[E_SkillState::R_Ready]->SetSkillProperty(this, E_SkillState::R_Ready);
	}

	ESkillTalentQuest = NewObject<UKerunESkillTalentQuest>();
	ESkillTalentQuest->InitTalentQuestProperty(this);
}

void AKerunCharacter::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	if (IsSelectedTalent[4][1])
	{
		if (!(endurance.Contains("KerunTalent41")))
		{
			endurance.Add("KerunTalent41", 20);
		}
	}

	if (IsSelectedTalent[4][2])
	{
		if (sidheRigelController->target)
		{
			if (!(speed.Contains("KerunTalent42")))
			{
				speed.Add("KerunTalent42", 300.f);
			}
		}
		else
		{
			if (speed.Contains("KerunTalent42"))
			{
				speed.Remove("KerunTalent42");
			}
		}
	}
}

void AKerunCharacter::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);
}

void AKerunCharacter::InitProperty()
{
	level = 1;
	range.Add("Debug", 200.f);
	attackDamage.Add("Debug", 5.f);
	attackSpeed.Add("Debug", 1.f);
	criticalRate.Add("Debug", 50);
	criticalDamage.Add("Debug", 50);

	MaxHP.Add("Debug", 100.f);
	generateHealthPoint.Add("Debug", 0.2f);
	MaxMP.Add("Debug", 100.f);
	lifeSteal.Add("Debug", 5.f);
	protectPower.Add("Debug", 20);

	defencePoint.Add("Debug", 100);

	speed.Add("Debug", 600.f);

	currentHP = GetMaxHP();
	currentMP = GetMaxMP();

	MaxExperience = 20;
}

void AKerunCharacter::Attack(AActor* target)
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
		AKerunAttackProjectile* Projectile = World->SpawnActor<AKerunAttackProjectile>(ProjectileClass, MuzzleLocation, MuzzleRotation, SpawnParams);
		if (Projectile)
		{
			// Set the projectile's initial trajectory.
			Projectile->Target = target;
			InitProjectileProperty(Projectile);
			
			//Kerun QSkill
			UKerunQSkill* QSkillRef = Cast<UKerunQSkill>(skills[E_SkillState::Q_Ready]);

			/*
			if (QSkillRef->IsWorking)
			{
				QSkillRef->AttackCount += 1;

				UE_LOG(LogTemp, Warning, TEXT("Add Attackcount :: %d"), QSkillRef->AttackCount);
			}
			*/
			

			if (IsSelectedTalent[0][2])
			{
				ADummyProjectile* TmpProjectile = Cast<ADummyProjectile>(Projectile);

				TmpProjectile->AttackDamage = GetAttackDamage() + QSkillRef->GetQDamage();
			}

			ImproveEStack(EAddAmount);

			//QSkillRef->ApplyTalentWhenFullComboHits(target);
			
		}
	}

	PlayAttackMontage(); 

	/*
	if (AnimInstance)
	{
		AnimInstance->PlayAttackMontage();
		UE_LOG(LogTemp, Warning, TEXT("AnimInstance is Not Null"));
	}
	*/
}

void AKerunCharacter::InitAttackProjectile()
{
	static ConstructorHelpers::FObjectFinder<UBlueprint> Projectile(TEXT("/Game/Heros/Kerun/BP_KerunAttackProjectile"));
	if (Projectile.Object)
	{
		ProjectileClass = (UClass*)Projectile.Object->GeneratedClass;
	}
}

void AKerunCharacter::SetCurrentHP(float _hp)
{
	Super::SetCurrentHP(_hp);

	/*
	if (KerunR2SkillRef->GetIsWorking())
	{
		if (KerunR2SkillRef->CheckCurrentHP(currentHP))
		{
			currentHP = KerunR2SkillRef->GetMinHP();
		}
	}
	*/
}

void AKerunCharacter::TakeDamage(float damage, AActor* damageCauser)
{
	if (IsSelectedTalent[6][1])
	{
		UKerunQSkill* QSkillRef = Cast<UKerunQSkill>(skills[E_SkillState::Q_Ready]);

		if (QSkillRef && QSkillRef->IsWorking)
		{
			damage *= (1 - KerunTalent61ReduceDamangeAmount);
		}
	}

	float tmp = damage - barrierAmount;
	DecreaseBarrierAmount(damage);

	currentHP -= tmp;

	if (ASidheRigelCharacter* causerCharacter = Cast<ASidheRigelCharacter>(damageCauser))
	{
		causerCharacter->LifeSteal(damage);
	}

	if (currentHP <= 0 && !isDie)
	{
		currentHP = 0;
		isDie = true;
		DieTime = 10.f;
		sidheRigelController->ChangeState(sidheRigelController->Die);

		UE_LOG(LogTemp, Error, TEXT("Dead!!"));
	}

	if (InGameUI != nullptr)
		InGameUI->CharacterStatus->UpdateHP();
	StatSummary->SetHPBar(currentHP / GetMaxHP());
}

void AKerunCharacter::PlayAttackMontage()
{
	UAnimInstance* AnimInstance = GetMesh()->GetAnimInstance();
	if (AnimInstance && AttackMontage)
	{
		AnimInstance->Montage_Play(AttackMontage);
		FName SectionName = FName();
		switch (AnimAttackCount)
		{
		case 0:
			SectionName = FName("Attack1");
			break;
		case 1:
			SectionName = FName("Attack2");
			break;
		case 2:
			SectionName = FName("Attack3");
			break;
		default:
			break;
		}
		AnimInstance->Montage_JumpToSection(SectionName, AttackMontage);
		
		AnimAttackCount++;
		if (AnimAttackCount >= 3)
		{
			AnimAttackCount -= 3;
		}
	}
}

void AKerunCharacter::PlayRSkillMontage()
{
	UAnimInstance* AnimInstance = GetMesh()->GetAnimInstance();
	if (AnimInstance && RSkillMontage)
	{
		AnimInstance->Montage_Play(RSkillMontage);
	}
}

void AKerunCharacter::PlayWSkillStartMontage()
{
	UAnimInstance* AnimInstance = GetMesh()->GetAnimInstance();
	if (AnimInstance && WSkillStartMontage)
	{
		AnimInstance->Montage_Play(WSkillStartMontage);
	}
}

void AKerunCharacter::PlayWSkillEndMontage()
{
	UAnimInstance* AnimInstance = GetMesh()->GetAnimInstance();
	if (AnimInstance && WSkillEndMontage)
	{
		AnimInstance->Montage_Play(WSkillEndMontage, 1.3f);
	}
}

void AKerunCharacter::PlayQSkillSound()
{
	if (QSkillSound)
	{
		UGameplayStatics::PlaySoundAtLocation(this, QSkillSound, this->GetActorLocation());
	}
}

void AKerunCharacter::PlayQSkillBuffSound()
{
	if (QSkillBuffSound)
	{
		UGameplayStatics::PlaySoundAtLocation(this, QSkillBuffSound, this->GetActorLocation());
	}
}

void AKerunCharacter::PlayWSkillSound()
{
	if (WSkillSound)
	{
		UGameplayStatics::PlaySoundAtLocation(this, WSkillSound, this->GetActorLocation());
	}
}

void AKerunCharacter::PlayWSkillLandingSound()
{
	if (WSkillLandingSound)
	{
		UGameplayStatics::PlaySoundAtLocation(this, WSkillLandingSound, this->GetActorLocation());
	}
}

void AKerunCharacter::PlayRSkillSound()
{
	if (RSkillSound)
	{
		UGameplayStatics::PlaySoundAtLocation(this, RSkillSound, this->GetActorLocation());
	}
}

void AKerunCharacter::UseParticleSystem(E_SkillState SkillState)
{
	UParticleSystem* Particle;

	switch (SkillState)
	{
	case W_Ready:
		Particle = WSkillParticle;
		break;
	case E_Ready:
		Particle = ESkillParticle;
		break;
	case R_Ready:
		Particle = RSkillParticle;
		break;
	default :
		Particle = nullptr;
		break;
	}

	if (Particle != nullptr)
	{
		FActorSpawnParameters SpawnParams;
		FTransform SpawnTransform;
		SpawnTransform.SetLocation(GetActorLocation());
		SpawnTransform.SetRotation(GetActorRotation().Quaternion());
		//SpawnTransform.SetScale3D(FVector(0.6f, 0.6f, 0.6));
		SpawnParams.Owner = this;

		UGameplayStatics::SpawnEmitterAtLocation(
			GetWorld(),
			Particle,
			SpawnTransform
		);
	}
}

void AKerunCharacter::UseQParticle(AActor* Target)
{
	UParticleSystem* Particle = QSkillParticle;

	if (Particle != nullptr)
	{
		FActorSpawnParameters SpawnParams;
		FTransform SpawnTransform;
		SpawnTransform.SetLocation(Target->GetActorLocation());
		SpawnTransform.SetRotation(Target->GetActorRotation().Quaternion());
		SpawnParams.Owner = this;

		UGameplayStatics::SpawnEmitterAtLocation(
			GetWorld(),
			Particle,
			SpawnTransform
		);

		UE_LOG(LogTemp, Warning , TEXT("QParticle Spawn"))
	}
}

void AKerunCharacter::ImproveEStack(int Count)
{

	ECurrentStack += Count;
	
	if (ECurrentStack > EMaxStack)
	{
		ECurrentStack = EMaxStack;
		if (IsSelectedTalent[2][0])
		{
			ESkillTalentQuest->OnCompleted();
		}
	}

	StartETimer();
}

void AKerunCharacter::SetEMaxStack(int32 Value)
{
	EMaxStack = Value;
}

void AKerunCharacter::SetEAddAmount(int32 Value)
{
	EAddAmount = Value;
}

void AKerunCharacter::InitKerunTalent()
{
	//level 1
	talentListArray[0].talentItems[0].talentName = "Kerun_1_1";
	talentListArray[0].talentItems[0].talentDescription = "Q 퀘스트 : 3번의 강화공격 모두 맞추기 10번 \n 보상 : 강화공격이 5번으로 증가";
	talentListArray[0].talentItems[0].imgPath = "/Game/Heros/Kerun/TalentIcons/Kerun_1_1";

	talentListArray[0].talentItems[1].talentName = "Kerun_1_2";
	talentListArray[0].talentItems[1].talentDescription = "Q 모두 맞추면 방어력 감소";
	talentListArray[0].talentItems[1].imgPath = "/Game/Heros/Kerun/TalentIcons/Kerun_1_2";

	talentListArray[0].talentItems[2].talentName = "Kerun_1_3";
	talentListArray[0].talentItems[2].talentDescription = "Q 강화 공격이 점점 강해짐";
	talentListArray[0].talentItems[2].imgPath = "/Game/Heros/Kerun/TalentIcons/Kerun_1_3";

	//level 4
	talentListArray[1].talentItems[0].talentName = "Kerun_2_1";
	talentListArray[1].talentItems[0].talentDescription = "W 퀘스트 : 막타치기 20회 \n 보상 : W 사용시 재생의 구슬 5개 획득";
	talentListArray[1].talentItems[0].imgPath = "/Game/Heros/Kerun/TalentIcons/Kerun_2_1";

	talentListArray[1].talentItems[1].talentName = "Kerun_2_2";
	talentListArray[1].talentItems[1].talentDescription = "W 둔화 추가";
	talentListArray[1].talentItems[1].imgPath = "/Game/Heros/Kerun/TalentIcons/Kerun_2_2";

	talentListArray[1].talentItems[2].talentName = "Kerun_2_3";
	talentListArray[1].talentItems[2].talentDescription = "W 보호막 추가";
	talentListArray[1].talentItems[2].imgPath = "/Game/Heros/Kerun/TalentIcons/Kerun_2_3";

	//level 7
	talentListArray[2].talentItems[0].talentName = "Kerun_3_1";
	talentListArray[2].talentItems[0].talentDescription = "E 퀘스트 : 재생의 구슬 최대치를 달성하기 \n 보상 : 재생의 구슬 최대치를 35로 증가시키고 재생의 구슬이 2배로 쌓임";
	talentListArray[2].talentItems[0].imgPath = "/Game/Heros/Kerun/TalentIcons/Kerun_3_1";

	talentListArray[2].talentItems[1].talentName = "Kerun_3_2";
	talentListArray[2].talentItems[1].talentDescription = "E 체력 재생량 증가";
	talentListArray[2].talentItems[1].imgPath = "/Game/Heros/Kerun/TalentIcons/Kerun_3_2";

	talentListArray[2].talentItems[2].talentName = "Kerun_3_3";
	talentListArray[2].talentItems[2].talentDescription = "E 스택 유지 시간 증가";
	talentListArray[2].talentItems[2].imgPath = "/Game/Heros/Kerun/TalentIcons/Kerun_3_3";

	//level 10
	talentListArray[3].talentItems[0].talentName = "Kerun_4_1";
	talentListArray[3].talentItems[0].talentDescription = "R 선택한 적 영웅을 기절시키고, 생명의 구슬 6개 획득";
	talentListArray[3].talentItems[0].imgPath = "/Game/Heros/Kerun/TalentIcons/Kerun_4_1";

	//level 13
	talentListArray[4].talentItems[0].talentName = "Kerun_5_1";
	talentListArray[4].talentItems[0].talentDescription = "패시브 흡혈 15%";
	talentListArray[4].talentItems[0].imgPath = "/Game/Heros/Kerun/TalentIcons/Kerun_5_1";

	talentListArray[4].talentItems[1].talentName = "Kerun_5_2";
	talentListArray[4].talentItems[1].talentDescription = "패시브 인내력 20%";
	talentListArray[4].talentItems[1].imgPath = "/Game/Heros/Kerun/TalentIcons/Kerun_5_2";

	talentListArray[4].talentItems[2].talentName = "Kerun_5_3";
	talentListArray[4].talentItems[2].talentDescription = "패시브 상대방 방향으로 이동할 때 이동속도 증가";
	talentListArray[4].talentItems[2].imgPath = "/Game/Heros/Kerun/TalentIcons/Kerun_5_3";

	//level 16
	talentListArray[5].talentItems[0].talentName = "Kerun_6_1";
	talentListArray[5].talentItems[0].talentDescription = "Q 확정 치명타 피해";
	talentListArray[5].talentItems[0].imgPath = "/Game/Heros/Kerun/TalentIcons/Kerun_6_1";

	talentListArray[5].talentItems[1].talentName = "Kerun_6_2";
	talentListArray[5].talentItems[1].talentDescription = "W 치유 감소 효과 적용";
	talentListArray[5].talentItems[1].imgPath = "/Game/Heros/Kerun/TalentIcons/Kerun_6_2";

	talentListArray[5].talentItems[2].talentName = "Kerun_6_3";
	talentListArray[5].talentItems[2].talentDescription = "W 아군한테 사용 가능";
	talentListArray[5].talentItems[2].imgPath = "/Game/Heros/Kerun/TalentIcons/Kerun_6_3";

	//level 20
	talentListArray[6].talentItems[0].talentName = "Kerun_7_1";
	talentListArray[6].talentItems[0].talentDescription = "패시브 일반공격이 주변 적에게도 100%의 피해";
	talentListArray[6].talentItems[0].imgPath = "/Game/Heros/Kerun/TalentIcons/Kerun_7_1";

	talentListArray[6].talentItems[1].talentName = "Kerun_7_2";
	talentListArray[6].talentItems[1].talentDescription = "Q 사용 중 받는 피해 30% 감소";
	talentListArray[6].talentItems[1].imgPath = "/Game/Heros/Kerun/TalentIcons/Kerun_7_2";

	talentListArray[6].talentItems[2].talentName = "Kerun_7_3";
	talentListArray[6].talentItems[2].talentDescription = "R 범위 내의 모든 적 영웅을 둔화, 밖으로 밀어내기";
	talentListArray[6].talentItems[2].imgPath = "/Game/Heros/Kerun/TalentIcons/Kerun_7_3";
}

float AKerunCharacter::GetEDuration()
{
	if (IsSelectedTalent[2][2])
	{
		return UpgradeEDuration;
	}
	else
	{
		return EDuration;
	}
}

float AKerunCharacter::GetEHealthRate()
{
	if (IsSelectedTalent[2][1])
	{
		return UpgradeEHealthRate;
	}
	else
	{
		return EHealthRate;
	}
}

void AKerunCharacter::StartETimer()
{
	QuitETimer();

	float GenerateAmount = ECurrentStack * GetEHealthRate();

	generateHealthPoint.Add("ESkill", GenerateAmount);

	GetWorld()->GetTimerManager().SetTimer(
		ETimer,
		FTimerDelegate::CreateLambda([&]() {
			QuitETimer();
			ECurrentStack = 0;
			UE_LOG(LogTemp, Warning, TEXT("ECurrentStack Initialized"));
			}),
		GetEDuration(),
		false
		);
}

void AKerunCharacter::QuitETimer()
{
	GetWorldTimerManager().ClearTimer(ETimer);
	generateHealthPoint.Remove("ESkill");
}
