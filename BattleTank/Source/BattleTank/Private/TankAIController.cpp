// Fill out your copyright notice in the Description page of Project Settings.

#include "TankAIController.h"
#include "BattleTank.h"
#include "Tank.h"
#include "TankAimingComponent.h"
#include "Engine/World.h"

void ATankAIController::SetPawn(APawn* InPawn)
{
	Super::SetPawn(InPawn);

	BindOnTankDeath(InPawn);
}

void ATankAIController::BindOnTankDeath(APawn * InPawn)
{
	if (InPawn)
	{
		ATank* PossessedTank = Cast<ATank>(InPawn);

		if (ensure(PossessedTank))
		{
			PossessedTank->OnDeath.AddUniqueDynamic(this, &ATankAIController::OnPossessedTankDeath);
		}
	}
}

void ATankAIController::OnPossessedTankDeath()
{
	if (GetPawn()) 
	{ 
		GetPawn()->DetachFromControllerPendingDestroy();
	}
}

void ATankAIController::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	APawn* PlayerTank = GetWorld()->GetFirstPlayerController()->GetPawn();
	APawn* ControlledTank = GetPawn();
	
	if (ensure(PlayerTank && ControlledTank)) 
	{  
		MoveToActor(PlayerTank, MinDistanceFromPlayerToKeep); 
		AimAtPlayer(ControlledTank, PlayerTank);
	}
}

void ATankAIController::AimAtPlayer(APawn * ControlledTank, APawn * PlayerTank)
{
	UTankAimingComponent* AimingComponent = ControlledTank->FindComponentByClass<UTankAimingComponent>();
	AimingComponent->AimAt(PlayerTank->GetActorLocation());

	if (AimingComponent->GetFiringState() == EFiringState::Locked) 
	{
		AimingComponent->Fire();
	}
}


