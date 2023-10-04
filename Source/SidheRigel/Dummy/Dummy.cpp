// Fill out your copyright notice in the Description page of Project Settings.


#include "Dummy.h"
#include "SidheRigel/SidheRigelCharacter.h"

// Sets default values
ADummy::ADummy()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	//스테틱 메쉬 컴포넌트 생성
	DummyMesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("DummyMeshComponent"));

	//기본 스텟 추가
	MaxHP.Add(TEXT("Default"), 100);
	currentHP = 100;
}

// Called when the game starts or when spawned
void ADummy::BeginPlay()
{
	Super::BeginPlay();
	
}

void ADummy::TakeDamage(float damage, AActor* damageCauser)
{
	currentHP -= damage;
	UE_LOG(LogTemp, Warning, TEXT("Remain HP is %f"), currentHP);

	if (ASidheRigelCharacter* causerCharacter = Cast<ASidheRigelCharacter>(damageCauser))
	{
		causerCharacter->LifeSteal(damage);
	}

	if (currentHP <= 0)
	{
		currentHP = 0;
		UE_LOG(LogTemp, Warning, TEXT("Dummy is Dead"));
	}
}

void ADummy::RestoreHP(float value)
{
	currentHP += value;
	float maxhp = 0;
	for (auto var : MaxHP)
	{
		maxhp += var.Value;
	}

	if (currentHP > maxhp)
	{
		currentHP = maxhp;
	}
}

float ADummy::GetHP()
{
	return currentHP;
}

void ADummy::Stun(float time)
{
	UE_LOG(LogTemp, Warning, TEXT("STUN : %fs"), time);
}

void ADummy::Stop(float time)
{
	UE_LOG(LogTemp, Warning, TEXT("STOP : %fs"), time);
}

void ADummy::Slow(float time, float value)
{
	UE_LOG(LogTemp, Warning, TEXT("SLOW : %fs, %f%%"), time, value);
}

void ADummy::Silence(float time)
{
	UE_LOG(LogTemp, Warning, TEXT("SILENCE : %fs"), time);
}

void ADummy::Airborne(float time)
{
	UE_LOG(LogTemp, Warning, TEXT("AIRBORNE : %fs"), time);
}

void ADummy::MoveVector(FVector Direction, float Force)
{
	moveDirection = Direction.GetSafeNormal();
	moveDirection *= FVector(1, 1, 0);
	moveForce = Force;
	moveCnt = 0;

	IsMoveVectorTrue = true;
}

// Called every frame
void ADummy::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	if (IsMoveVectorTrue)
	{
		FVector Location = GetActorLocation();
		Location += moveDirection * (moveForce / 10) * DeltaTime;
		SetActorLocation(Location);
		moveCnt++;
		if (moveCnt >= 10)
		{
			moveDirection = FVector::ZeroVector;
			moveForce = 0;
			moveCnt = 0;

			IsMoveVectorTrue = false;
		}
	}
}

