// Fill out your copyright notice in the Description page of Project Settings.


#include "MagicBezierCarrier.h"
#include "MagicBezierFunctions.h"

// Sets default values
AMagicBezierCarrier::AMagicBezierCarrier()
{
 	// Set this pawn to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

}

// Called when the game starts or when spawned
void AMagicBezierCarrier::BeginPlay()
{
	Super::BeginPlay();
	
}

// Called every frame
void AMagicBezierCarrier::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	float T = DeltaTime * CarrierSpeed/ Length;
	FVector Location = MagicBezierFunctions::CubicBezierLocation(P0, P1, P2, P3, T);

	this->SetActorLocation(Location);

}

// Called to bind functionality to input
void AMagicBezierCarrier::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);

}

void AMagicBezierCarrier::CalculateControlPointsCubicBezier()
{
	if (LastGate != nullptr)
	{
		P0 = LastGate->GetActorLocation();
		P1 = LastGate->GetActorForwardVector();
		BezierStrength = LastGate->BezierStrength;
	} else
	{
		P0 = this->GetActorLocation();
		P1 = this->GetActorForwardVector();
	}
	
	P1 = P1.ForwardVector;
	P1.X = P1.X * BezierStrength / sqrt(pow(P1.X, 2) + pow(P1.Y, 2)); // Forward vector is a unit vector
	P1 = LastGate->GetTransform().TransformPosition(P1);

	

	if (NextGate != nullptr)
	{
		P3 = NextGate->GetActorLocation();

		P2 = NextGate->GetActorForwardVector();
		P2 = P2.BackwardVector;
		P2.X = P2.X * BezierStrength / sqrt(pow(P2.X, 2) + pow(P2.Y, 2));
		P2 = NextGate->GetTransform().TransformPosition(P2);

	}
}

float AMagicBezierCarrier::CalculateLength(FVector P0, FVector P1, FVector P2, FVector P3, float SegmentInterval = 50)
{
	const auto PointsApproximation = MagicBezierFunctions::CubicBezierCurve(P0, P1, P2, P3, 1.0 / 50);
	const auto LengthApproximation = MagicBezierFunctions::CubicBezierCurveLength(PointsApproximation);
	const auto SecondLengthApproximation = MagicBezierFunctions::CubicBezierCurve(P0, P1, P2, P3, (1.0 / LengthApproximation) * SegmentInterval / 10);
	Length = MagicBezierFunctions::CubicBezierCurveLength(SecondLengthApproximation);
}



