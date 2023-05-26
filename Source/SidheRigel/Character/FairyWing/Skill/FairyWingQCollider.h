// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "FairyWingQCollider.generated.h"

UCLASS()
class SIDHERIGEL_API AFairyWingQCollider : public AActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	AFairyWingQCollider();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

public:
	UPROPERTY()
		AActor* Target;
	UPROPERTY()
		AActor* colliderOwner; 
	UPROPERTY(EditAnywhere)
		class USphereComponent* CollisionComponent;

protected:
	UFUNCTION()
		void OnColliderOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult);

private:
	UPROPERTY(EditAnywhere)
		class UStaticMeshComponent* ColliderMesh;
};
