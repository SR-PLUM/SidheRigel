#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "TowerDestroyParticle.generated.h"

UCLASS()
class SIDHERIGEL_API ATowerDestroyParticle : public AActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	ATowerDestroyParticle();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	float particleDuration = 1.f;
};
