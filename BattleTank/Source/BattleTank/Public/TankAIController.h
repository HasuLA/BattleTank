// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "AIController.h"
#include "TankAIController.generated.h"

class UTankAimingComponent;
class ATank;

UCLASS()
class BATTLETANK_API ATankAIController : public AAIController
{
	GENERATED_BODY()
protected:

	UPROPERTY(EditDefaultsOnly, Category = "Setup")
	float MinDistanceFromPlayerToKeep = 8000;

private:
	virtual void SetPawn(APawn* InPawn) override;
	void BindOnTankDeath(APawn * InPawn);

	UFUNCTION()
	void OnPossessedTankDeath();

	virtual void Tick(float DeltaTime) override;

	void AimAtPlayer(APawn * ControlledTank, APawn * PlayerTank);
}; 
