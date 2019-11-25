// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Pawn.h"
#include "MagicBezierGate.h"
#include "MagicBezierCarrier.generated.h"

UCLASS()
class MAGICBEZIER_API AMagicBezierCarrier : public APawn
{
	GENERATED_BODY()

public:
	// Sets default values for this pawn's properties
	AMagicBezierCarrier();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	// Called to bind functionality to input
	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Magic Bezier")
	AMagicBezierGate* NextGate;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Magic Bezier")
	FVector P0;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Magic Bezier")
	FVector P1;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Magic Bezier")
	FVector P2;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Magic Bezier")
	FVector P3;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Magic Bezier")
	float Length = 0;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Magic Bezier")
	float InitialBezierStrength = 1000;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Magic Bezier")
	float ForwardBezierStrength = 1000;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Magic Bezier")
	float BackwardBezierStrength = 1000;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Magic Bezier")
	float CarrierSpeed = 200;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Magic Bezier")
	UStaticMesh* Carrier;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Magic Bezier")
	float SegmentInterval = 50;

	UStaticMeshComponent* CarrierVisual;

	float ProgressAlongCurve = 0.0f;

private:
	void CalculateControlPointsCubicBezier();
	
};
