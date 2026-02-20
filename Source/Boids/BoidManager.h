// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "Components/BoxComponent.h"
#include "Boid.h"
#include "BoidManager.generated.h"

UCLASS()
class BOIDS_API ABoidManager : public AActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	ABoidManager();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

	FScriptDelegate OnBoundExit;

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

protected:
	UFUNCTION()
	void SpawnBoids();
	UFUNCTION()
	FVector GetCenterMassVelocity(const ABoid* Boid);
	UFUNCTION()
	FVector GetMoveAwayVelocity(const ABoid* Boid);
	UFUNCTION()
	FVector GetMatchingVelocity(const ABoid* Boid);
	UFUNCTION()
	void BoundCheck(ABoid* Boid);
	UFUNCTION()
	FVector GetWind(const ABoid* Boid);

protected:
	UPROPERTY(EditAnywhere)
	TSubclassOf<ABoid> BoidClass;
	UPROPERTY(EditAnywhere)
	int32 BoidAmount = 200;
	UPROPERTY(EditAnywhere, meta = (ClampMin = 0.0f, ClampMax = 100.0f, UIMin = 0.0f, UIMax = 100.0f))
	float CenterMassMovementPercent = 1.0f;
	UPROPERTY(EditAnywhere)
	float DistanceFromBoid = 50.0f;
	UPROPERTY(EditAnywhere, meta = (ClampMin = 0.0f, ClampMax = 100.0f, UIMin = 0.0f, UIMax = 100.0f))
	float MatchVelocityPercent = 1.25f;
	UPROPERTY(EditAnywhere)
	FVector Wind = FVector(10,0,0);
	UPROPERTY(EditAnywhere)
	bool BorderEnabled;
	UPROPERTY(EditAnywhere)
	bool DrawDebug;

	UPROPERTY(BlueprintReadOnly)
	TArray<ABoid*> BoidList;

	UPROPERTY(VisibleAnywhere)
	UBoxComponent* BoundingBox;
};
