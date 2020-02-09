// Fill out your copyright notice in the Description page of Project Settings.

#include "TankPlayerController.h"
#include "BattleTank.h"
#include "Tank.h"
#include "TankAimingComponent.h"
#include "Engine/World.h"
#include "Camera/PlayerCameraManager.h"

void ATankPlayerController::BeginPlay()
{
	Super::BeginPlay();

	CacheAimingComponent();
	OnAimingComponentFound(ControlledTankAimingComp);
}

void ATankPlayerController::CacheAimingComponent()
{
	ControlledTankAimingComp = GetPawn()->FindComponentByClass<UTankAimingComponent>();
	ensure(ControlledTankAimingComp);
}

void ATankPlayerController::SetPawn(APawn* InPawn)
{
	Super::SetPawn(InPawn);
	
	BindOnTankDeath(InPawn);
}

void ATankPlayerController::BindOnTankDeath(APawn * InPawn)
{
	if (InPawn)
	{
		ATank* PossessedTank = Cast<ATank>(InPawn);
		if (ensure(PossessedTank))
		{
			PossessedTank->OnDeath.AddUniqueDynamic(this, &ATankPlayerController::OnPossessedTankDeath);
		}
	}
}

void ATankPlayerController::OnPossessedTankDeath()
{
	StartSpectatingOnly();
}

// Called every frame
void ATankPlayerController::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	AimTowardsCrosshair();
}

void ATankPlayerController::AimTowardsCrosshair()
{
	if (CanAim())
	{
		FVector OutHitLocation;
		bool bGotHitLocation = GetSightRayHitLocation(OutHitLocation);

		if (bGotHitLocation) 
		{
			ControlledTankAimingComp->AimAt(OutHitLocation);
		}
	}
}

bool ATankPlayerController::CanAim() const
{
	return GetPawn() && ControlledTankAimingComp;
}

// Get world location of linetrace through crosshair, true if hits landscape
bool ATankPlayerController::GetSightRayHitLocation(FVector& OutHitLocation) const
{
	// Finding the crosshair position in pixel coordinates
	int32 OutViewportSizeX, OutViewportSizeY;
	GetViewportSize(OutViewportSizeX, OutViewportSizeY);

	// Crosshair location is calculated relatively to viewport upper left corner, considering viewport size and crosshair relative position.
	FVector2D CrosshairLocationOnScreen = FVector2D(OutViewportSizeX * CrosshairXScreenRelLocation, OutViewportSizeY * CrosshairYScreenRelLocation);
	
	FVector LookDirection;
	if (GetLookDirection(CrosshairLocationOnScreen, LookDirection)) // "De-project" the screen position of the crosshair to a world direction
	{
		// Line-trace along that LookDirection and see what we hit (up to max range)
		return GetLookVectorHitLocation(LookDirection, OutHitLocation);
	}

	return false;
}

bool ATankPlayerController::GetLookDirection(FVector2D ScreenLocation, FVector& LookDirection) const
{
	FVector CameraWorldLocation;
	return DeprojectScreenPositionToWorld(
		ScreenLocation.X,
		ScreenLocation.Y,
		CameraWorldLocation,
		LookDirection
	);
}

bool  ATankPlayerController::GetLookVectorHitLocation(FVector LookDirection, FVector& OutHitLocation) const 
{
	FHitResult OutHitResult;
	FVector StartLocation = PlayerCameraManager->GetCameraLocation();
	FVector EndLocation = StartLocation + (LookDirection * LineTraceRange);

	if (HasVisibleHitLocation(OutHitResult, StartLocation, EndLocation))
	{
		OutHitLocation = OutHitResult.Location;
		return true;
	}

	OutHitLocation = FVector::ZeroVector;
	return false;
}

bool ATankPlayerController::HasVisibleHitLocation(FHitResult& OutHitResult, const FVector& StartLocation, const FVector& EndLocation) const
{
	return GetWorld()->LineTraceSingleByChannel(
		OutHitResult,
		StartLocation,
		EndLocation,
		ECollisionChannel::ECC_Visibility
	);
}