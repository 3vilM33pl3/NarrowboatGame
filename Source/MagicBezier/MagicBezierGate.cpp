#include "MagicBezierGate.h"
#include "ConstructorHelpers.h"
#include "Components/StaticMeshComponent.h"
#include "ComponentReregisterContext.h"
#include "Editor.h"
#include <Runtime\Engine\Classes\Kismet\KismetMathLibrary.h>

void AMagicBezierGate::CalculateControlPoints()
{
	P0 = GetActorLocation();

	P1 = GetActorForwardVector();
	P1.X = P1.X * BezierStrength; // Forward vector is a unit vector
	P1 = GetTransform().TransformPosition(P1);
	
	if(NextGate != nullptr)
	{
		P3 = NextGate->GetActorLocation();
		
		P2 = NextGate->GetActorForwardVector();
		P2.X = P2.X * BezierStrength * -1;
		P2 = NextGate->GetTransform().TransformPosition(P2);
	}
}

// Sets default values
AMagicBezierGate::AMagicBezierGate()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

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
	
}

// Called when the game starts or when spawned
void AMagicBezierGate::BeginPlay()
{
	Super::BeginPlay();	
}

// Called every frame
void AMagicBezierGate::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

void AMagicBezierGate::OnConstruction(const FTransform& Transform)
{
	CalculateControlPoints();
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

