// Fill out your copyright notice in the Description page of Project Settings.

#include "TankAIController.h"
#include "BattleTank.h"
#include "Tank.h"
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
	ATank* PlayerTank = Cast<ATank>(GetWorld()->GetFirstPlayerController()->GetPawn());
	ATank* ControlledTank = Cast<ATank>(GetPawn());
	if (ensure(PlayerTank)) {
		// move towards the player
		MoveToActor(PlayerTank, AcceptanceRadius); //TODO check radius is in santimeters

		// Aim towards the player
		ControlledTank->AimAt(PlayerTank->GetActorLocation());
		
		ControlledTank->Fire(); // TODO limit firing rate
	}
}


