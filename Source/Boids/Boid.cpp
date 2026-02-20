// Fill out your copyright notice in the Description page of Project Settings.


#include "Boid.h"
#include "Kismet/KismetMathLibrary.h"

// Sets default values
ABoid::ABoid()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	RootComponent = CreateDefaultSubobject<USceneComponent>(TEXT("RootComponent"));
	Mesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("Mesh"));

	Mesh->SetupAttachment(RootComponent);
}

// Called when the game starts or when spawned
void ABoid::BeginPlay()
{
	Super::BeginPlay();
	
	Velocity = FVector(FMath::FRandRange(-3.0f, 3.0f), FMath::FRandRange(-3.0f, 3.0f), FMath::FRandRange(-3.0f, 3.0f));
}

// Called every frame
void ABoid::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	Mesh->SetWorldRotation(UKismetMathLibrary::FindLookAtRotation(GetActorLocation(), GetActorLocation() + Velocity));
}

