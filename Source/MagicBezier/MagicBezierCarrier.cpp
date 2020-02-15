// Fill out your copyright notice in the Description page of Project Settings.

#include "MagicBezierCarrier.h"
#include  "Components/StaticMeshComponent.h"
#include "MagicBezierFunctions.h"
#include "Engine/StaticMesh.h"
#include "ConstructorHelpers.h"

	// Sets default values
AMagicBezierCarrier::AMagicBezierCarrier()
{
	// Set this pawn to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	// Setup carrier
	USceneComponent* EmptyComponent = CreateDefaultSubobject<USceneComponent>(TEXT("RootComponent"));
	RootComponent = EmptyComponent;

	CarrierVisual = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("VisualRepresentation"));
	CarrierVisual->SetupAttachment(RootComponent);
	static ConstructorHelpers::FObjectFinder<UStaticMesh> CarrierVisualAsset(TEXT("/Game/MagicBezier/SM_Boat.SM_Boat"));
	if (CarrierVisualAsset.Succeeded())
	{
		UE_LOG(LogTemp, Log, TEXT("Carrier static mesh loaded"));

		Carrier = CarrierVisualAsset.Object;
		CarrierVisual->SetStaticMesh(Carrier);
		CarrierVisual->SetCollisionProfileName(TEXT("NoCollision"));
	}
	else
	{
		UE_LOG(LogTemp, Warning, TEXT("Failed to load boat static mesh"));
	}

}

// Called when the game starts or when spawned
void AMagicBezierCarrier::BeginPlay()
{
	Super::BeginPlay();
	CalculateControlPointsCubicBezier();
	ForwardBezierStrength = InitialBezierStrength;
	if (NextGate != nullptr)
	{
		BackwardBezierStrength = NextGate->BackwardBezierStrength;
	}

	Length = MagicBezierFunctions::CubicBezierLengthEstimate(P0, P1, P2, P3, SegmentInterval);
	this->SetActorLocation(this->GetActorLocation());
}

// Called every frame
void AMagicBezierCarrier::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	if (ProgressAlongCurve < 1.0f)
	{
		ProgressAlongCurve = ProgressAlongCurve + DeltaTime * CarrierSpeed / Length;
		const FVector Location = MagicBezierFunctions::CubicBezierLocation(P0, P1, P2, P3, ProgressAlongCurve);
		this->SetActorLocation(Location);

		const FVector Rotation = MagicBezierFunctions::CubicBezierCurveDerivative(P0, P1, P2, P3, ProgressAlongCurve);
		this->SetActorRotation(Rotation.Rotation());
	}
	else
	{
		if (NextGate != nullptr && NextGate->NextGate != nullptr)
		{
			auto const CurrentGate = NextGate;
			Length = CurrentGate->Length;
			ForwardBezierStrength = CurrentGate->ForwardBezierStrength;
			BackwardBezierStrength = CurrentGate->NextGate->BackwardBezierStrength;
			NextGate = CurrentGate->NextGate;
			ProgressAlongCurve = 0.0;
			CalculateControlPointsCubicBezier();
		}
	}

}

// Called to bind functionality to input
void AMagicBezierCarrier::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);

}

void AMagicBezierCarrier::CalculateControlPointsCubicBezier()
{
	if (NextGate != nullptr)
	{
		P0 = this->GetActorLocation();
		P1 = this->GetActorForwardVector();
		//BezierStrength = NextGate->BezierStrength;

		P1 = P1.ForwardVector;
		P1.X = P1.X * ForwardBezierStrength / sqrt(pow(P1.X, 2) + pow(P1.Y, 2)); // Forward vector is a unit vector
		P1 = this->GetTransform().TransformPosition(P1);

		P3 = NextGate->GetActorLocation();

		P2 = NextGate->GetActorForwardVector();
		P2 = P2.BackwardVector;
		P2.X = P2.X * NextGate->BackwardBezierStrength / sqrt(pow(P2.X, 2) + pow(P2.Y, 2));
		P2 = NextGate->GetTransform().TransformPosition(P2);

	}
}




