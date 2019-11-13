#include "MagicBezierGate.h"
#include "ConstructorHelpers.h"
#include "Components/StaticMeshComponent.h"
#include "ComponentReregisterContext.h"
#include "Editor.h"
#include <Runtime\Engine\Classes\Kismet\KismetMathLibrary.h>
#include "MagicBezierFunctions.h"
#include "DrawDebugHelpers.h"

void AMagicBezierGate::CalculateControlCubicBezier()
{
	P0 = GetActorLocation();

	P1 = GetActorForwardVector();
	P1 = P1.ForwardVector;
	P1.X = P1.X * BezierStrength / sqrt(pow(P1.X,2) + pow(P1.Y,2)); // Forward vector is a unit vector
	P1 = GetTransform().TransformPosition(P1);

	if(NextGate != this)
	{
		P3 = NextGate->GetActorLocation();
		
		P2 = NextGate->GetActorForwardVector();
		P2 = P2.BackwardVector;
		P2.X = P2.X * BezierStrength / sqrt(pow(P2.X,2)+pow(P2.Y,2));
		P2 = NextGate->GetTransform().TransformPosition(P2);
		
		const auto PointsApproximation = MagicBezierFunctions::CubicBezierCurve(P0, P1, P2, P3, 1.0 / 50);
		const auto LengthApproximation = MagicBezierFunctions::CubicBezierCurveLength(PointsApproximation);
		CubicBezierCurvePoints = MagicBezierFunctions::CubicBezierCurve(P0, P1, P2, P3, (1.0 / LengthApproximation)*DebugPointsDistance/10) ;
		Length = MagicBezierFunctions::CubicBezierCurveLength(CubicBezierCurvePoints);
		
	}

}

// Sets default values
AMagicBezierGate::AMagicBezierGate()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick =  true;

	USceneComponent* EmptyComponent = CreateDefaultSubobject<USceneComponent>(TEXT("RootComponent"));
	RootComponent = EmptyComponent;
	
	// Set default gate static mesh
	GateVisual = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("VisualRepresentation"));
	GateVisual->SetupAttachment(RootComponent);
	static ConstructorHelpers::FObjectFinder<UStaticMesh> GateVisualAsset(TEXT("/Game/MagicBezier/SM_Gate.SM_Gate"));
	if (GateVisualAsset.Succeeded())
	{
		UE_LOG(LogTemp, Log, TEXT("Gate static mesh loaded"));

		Gate = GateVisualAsset.Object;
		GateVisual->SetStaticMesh(Gate);
		GateVisual->SetCollisionProfileName(TEXT("NoCollision"));
	}
	else
	{
		UE_LOG(LogTemp, Warning, TEXT("Failed to load gate static mesh"));
	}

	NextGate = this; // If there is no next gate point to itself (default)
	
}

// Called when the game starts or when spawned
void AMagicBezierGate::BeginPlay()
{
	CalculateControlCubicBezier();
	Super::BeginPlay();	
}

// Called every frame
void AMagicBezierGate::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	if(NextGate != this)
	{
		for (FVector L : CubicBezierCurvePoints)
		{
			DrawDebugPoint(GetWorld(), L, 10, FColor(255, 0, 0), false, 0);
			DrawDebugDirectionalArrow(GetWorld(), P0, P1, 50, FColor(255, 255, 0), false,-1,0,10);
			DrawDebugDirectionalArrow(GetWorld(), P3, P2, 50, FColor(0, 255, 255), false, -1, 0, 10);
		}
	}
	
	
	

}

void AMagicBezierGate::OnConstruction(const FTransform& Transform)
{
	//CalculateControlCubicBezier();
	Super::OnConstruction(Transform);
}

#if WITH_EDITOR

void AMagicBezierGate::PostEditChangeProperty(FPropertyChangedEvent& PropertyChangedEvent)
{
	TArray<UActorComponent*> MyComponents;
	GetComponents(MyComponents);
	
	FName PropertyName = (PropertyChangedEvent.Property != nullptr) ? PropertyChangedEvent.Property->GetFName() : NAME_None;
	if ((PropertyName == GET_MEMBER_NAME_CHECKED(AMagicBezierGate, Gate)))
	{
		FMultiComponentReregisterContext ReregisterContext(MyComponents);

		for (UActorComponent* Comp : MyComponents)
		{
			if (UStaticMeshComponent* MeshComp = Cast<UStaticMeshComponent>(Comp))
			{
				GateVisual->SetStaticMesh(Gate); // Update the component to the new mesh
				GEditor->EditorUpdateComponents();
			}
		}
	}

	Super::PostEditChangeProperty(PropertyChangedEvent);
}
#endif

