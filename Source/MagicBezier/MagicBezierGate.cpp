#include "MagicBezierGate.h"
#include "ConstructorHelpers.h"
#include "Components/SphereComponent.h"
#include "Components/StaticMeshComponent.h"

// Sets default values
AMagicBezierGate::AMagicBezierGate()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	USphereComponent* SphereComponent = CreateDefaultSubobject<USphereComponent>(TEXT("RootComponent"));
	RootComponent = SphereComponent;
	SphereComponent->SetCollisionProfileName(TEXT("NoCollision"));
	
	// Set default gate static mesh
	GateVisual = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("VisualRepresentation"));
	GateVisual->SetupAttachment(RootComponent);
	static ConstructorHelpers::FObjectFinder<UStaticMesh> GateVisualAsset(TEXT("/Game/MagicBezier/SM_Gate.SM_Gate"));
	if (GateVisualAsset.Succeeded())
	{
		UE_LOG(LogTemp, Log, TEXT("Gate static mesh loaded"));

		Gate = GateVisualAsset.Object;
		GateVisual->SetStaticMesh(Gate);	
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

#if WITH_EDITOR
void AMagicBezierGate::PostEditChangeProperty(FPropertyChangedEvent& PropertyChangedEvent)
{
	GateVisual = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("VisualRepresentation"));
	GateVisual->SetupAttachment(RootComponent);
	GateVisual->SetStaticMesh(Gate);
}
#endif

