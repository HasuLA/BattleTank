// Fill out your copyright notice in the Description page of Project Settings.

#include "TankAIController.h"
#include "BattleTank.h"
#include "TankAimingComponent.h"
#include "Engine/World.h"
// Depends on movement component via pathfinding system

void ATankAIController::BeginPlay()
{
	Super::BeginPlay();
}

// Called every frame
void ATankAIController::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
	APawn* PlayerTank = GetWorld()->GetFirstPlayerController()->GetPawn();
	APawn* ControlledTank = GetPawn();
	
	
	if (!ensure(PlayerTank && ControlledTank)) { return; }
	// move towards the player
	MoveToActor(PlayerTank, AcceptanceRadius); //TODO check radius is in santimeters
	// Aim towards the player
	UTankAimingComponent* AimingComponent = ControlledTank->FindComponentByClass<UTankAimingComponent>();
	AimingComponent->AimAt(PlayerTank->GetActorLocation());
	
	// TODO Fix Firing
	//ControlledTank->Fire(); // TODO limit firing rate
	
}


