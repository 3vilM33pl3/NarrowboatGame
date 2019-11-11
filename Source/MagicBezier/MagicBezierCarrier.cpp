// Fill out your copyright notice in the Description page of Project Settings.


#include "MagicBezierCarrier.h"

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

}

// Called to bind functionality to input
void AMagicBezierCarrier::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);

}

