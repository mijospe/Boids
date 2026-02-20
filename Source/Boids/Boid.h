// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "Boid.generated.h"

UCLASS()
class BOIDS_API ABoid : public AActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	ABoid();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;


protected:
	UPROPERTY(VisibleAnywhere)
	UStaticMeshComponent* Mesh;

public:
	UPROPERTY(VisibleAnywhere)
	FVector Velocity;
	UPROPERTY(VisibleAnywhere)
	FVector MinVelocity{ 1,1,1 };
	UPROPERTY(VisibleAnywhere)
	FVector MaxVelocity{ 20,20,20 };

};
