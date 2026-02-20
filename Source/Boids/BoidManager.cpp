// Fill out your copyright notice in the Description page of Project Settings.

#include "BoidManager.h"
#include "Kismet/GameplayStatics.h"

// Sets default values
ABoidManager::ABoidManager()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	RootComponent = CreateDefaultSubobject<USceneComponent>(TEXT("RootComponent"));

	BoundingBox = CreateDefaultSubobject<UBoxComponent>(TEXT("Bounding Box"));
	BoundingBox->SetupAttachment(RootComponent);
}

// Called when the game starts or when spawned
void ABoidManager::BeginPlay()
{
	Super::BeginPlay();

	SpawnBoids();
}

// Called every frame
void ABoidManager::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
	
	if (DrawDebug)
	{
		UKismetSystemLibrary::DrawDebugBox(GetWorld(), BoundingBox->GetComponentLocation(), BoundingBox->Bounds.GetBox().GetExtent(), FColor::Red, FRotator::ZeroRotator);
	}

	if (BoidList.Num() <= 0) return;

	for (ABoid* Boid : BoidList)
	{
		FVector Velocity1 = GetCenterMassVelocity(Boid);
		FVector Velocity2 = GetMoveAwayVelocity(Boid);
		FVector Velocity3 = GetMatchingVelocity(Boid);

		FVector NewVelocity =  Boid->Velocity + (Velocity1 + Velocity2 + Velocity3 + GetWind(Boid)) * GetWorld()->GetDeltaSeconds();

		Boid->Velocity = ClampVector(NewVelocity, Boid->MinVelocity, Boid->MaxVelocity);

		Boid->SetActorLocation(Boid->GetActorLocation() + Boid->Velocity);

		if (BorderEnabled) BoundCheck(Boid);

		if (Boid->GetActorLocation().X > GetActorLocation().X + BoundingBox->Bounds.GetBox().GetExtent().X) { ; }

	}
}

// Spawn Boids
void ABoidManager::SpawnBoids()
{
	FActorSpawnParameters SpawnParams;

	// Spawn selected amount of Boids
	for (int32 i = 0; i < BoidAmount; ++i)
	{
		// Get the Bounding Box Location and Extend
		FVector SpawnLocation = BoundingBox->GetComponentLocation();
		FVector BoxExtent = BoundingBox->Bounds.GetBox().GetExtent();

		// Choose a random location within the box to spawn the boids at
		SpawnLocation = SpawnLocation + FVector(
			FMath::FRandRange(-BoxExtent.X, BoxExtent.X),
			FMath::FRandRange(-BoxExtent.Y, BoxExtent.Y),
			FMath::FRandRange(-BoxExtent.Z, BoxExtent.Z)
		);

		ABoid* SpawnedBoid = GetWorld()->SpawnActor<ABoid>(BoidClass, SpawnLocation, FRotator::ZeroRotator, SpawnParams);
		BoidList.Add(SpawnedBoid);
	}
}

// Rule 1 (Movement towards Center of Mass)
FVector ABoidManager::GetCenterMassVelocity(const ABoid* Boid)
{
	FVector CenterMass;

	for (const ABoid* BoidInList : BoidList)
	{
		if (BoidInList != Boid) CenterMass += BoidInList->GetActorLocation();
	}

	// Get the average location of them all
	CenterMass = CenterMass / (BoidList.Num() - 1);

	return (CenterMass - Boid->GetActorLocation()) * (CenterMassMovementPercent * 0.01);
}

// Rule 2 (Move away from other Boids)
FVector ABoidManager::GetMoveAwayVelocity(const ABoid* Boid)
{
	FVector MoveAwayDirection = FVector(0);

	for (const ABoid* BoidInList : BoidList)
	{
		if (BoidInList != Boid && FMath::Abs((BoidInList->GetActorLocation() - Boid->GetActorLocation()).Length()) < DistanceFromBoid)
		{
			MoveAwayDirection = MoveAwayDirection - (BoidInList->GetActorLocation() - Boid->GetActorLocation());
		}
	}

	return MoveAwayDirection;
}

// Rule 3 (Match Velocity with nearby Boids
FVector ABoidManager::GetMatchingVelocity(const ABoid* Boid)
{
	FVector MatchingVelocity;

	for (const ABoid* BoidInList : BoidList)
	{
		if (BoidInList != Boid) MatchingVelocity = MatchingVelocity + BoidInList->Velocity;
	}

	MatchingVelocity = MatchingVelocity / (BoidList.Num() - 1);

	return (MatchingVelocity - Boid->Velocity) * (MatchVelocityPercent * 0.01f);
}

// Rule 4 (Check Bounding Box)
void ABoidManager::BoundCheck(ABoid* Boid)
{
	// X
	if (Boid->GetActorLocation().X > GetActorLocation().X + BoundingBox->Bounds.GetBox().GetExtent().X)
	{
		Boid->SetActorLocation(FVector(GetActorLocation().X - BoundingBox->Bounds.GetBox().GetExtent().X, Boid->GetActorLocation().Y, Boid->GetActorLocation().Z));
	}
	if (Boid->GetActorLocation().X < GetActorLocation().X - BoundingBox->Bounds.GetBox().GetExtent().X)
	{
		Boid->SetActorLocation(FVector(GetActorLocation().X + BoundingBox->Bounds.GetBox().GetExtent().X, Boid->GetActorLocation().Y, Boid->GetActorLocation().Z));
	}
	// Y
	if (Boid->GetActorLocation().Y > GetActorLocation().Y + BoundingBox->Bounds.GetBox().GetExtent().Y)
	{
		Boid->SetActorLocation(FVector(Boid->GetActorLocation().X, GetActorLocation().Y - BoundingBox->Bounds.GetBox().GetExtent().Y, Boid->GetActorLocation().Z));
	}
	if (Boid->GetActorLocation().Y < GetActorLocation().Y - BoundingBox->Bounds.GetBox().GetExtent().Y)
	{
		Boid->SetActorLocation(FVector(Boid->GetActorLocation().X, GetActorLocation().Y + BoundingBox->Bounds.GetBox().GetExtent().Y, Boid->GetActorLocation().Z));

	}
	// Z
	if (Boid->GetActorLocation().Z > GetActorLocation().Z + BoundingBox->Bounds.GetBox().GetExtent().Z)
	{
		Boid->SetActorLocation(FVector(Boid->GetActorLocation().X, Boid->GetActorLocation().Y, GetActorLocation().Z - BoundingBox->Bounds.GetBox().GetExtent().Z));
	}
	if (Boid->GetActorLocation().Z < GetActorLocation().Z - BoundingBox->Bounds.GetBox().GetExtent().Z)
	{
		Boid->SetActorLocation(FVector(Boid->GetActorLocation().X, Boid->GetActorLocation().Y, GetActorLocation().Z + BoundingBox->Bounds.GetBox().GetExtent().Z));
	}
}

FVector ABoidManager::GetWind(const ABoid* Boid)
{
	return Wind;
}