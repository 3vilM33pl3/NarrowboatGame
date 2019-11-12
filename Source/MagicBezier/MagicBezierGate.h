#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "MagicBezierGate.generated.h"

UCLASS()
class MAGICBEZIER_API AMagicBezierGate : public AActor
{
	GENERATED_BODY()
	
public:
	void CalculateControlPoints();
	// Sets default values for this actor's properties
	AMagicBezierGate();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	void OnConstruction(const FTransform& Transform) override;

	
#if WITH_EDITOR
	void PostEditChangeProperty(FPropertyChangedEvent& PropertyChangedEvent) override;
#endif	
	
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category="Magic Bezier")
	FVector P0;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Magic Bezier")
	FVector P1;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Magic Bezier")
	FVector P2;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Magic Bezier")
	FVector P3;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Magic Bezier")
	float BezierStrength = 1000;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Magic Bezier")
	AMagicBezierGate* NextGate;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Magic Bezier")
	UStaticMesh* Gate;

	UStaticMeshComponent* GateVisual;
};
