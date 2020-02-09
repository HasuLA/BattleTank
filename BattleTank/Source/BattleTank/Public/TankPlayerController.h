// Copyright EmbraceIT Ltd.

#pragma once

#include "GameFramework/PlayerController.h"
#include "TankPlayerController.generated.h"

class UTankAimingComponent;
class ATank;

UCLASS()
class BATTLETANK_API ATankPlayerController : public APlayerController
{
	GENERATED_BODY()
	
protected:
	UFUNCTION(BlueprintImplementableEvent, Category = "Setup")
	void OnAimingComponentFound(UTankAimingComponent* AimCompRef);

private:
	virtual void BeginPlay() override;
	void CacheAimingComponent();

	virtual void Tick(float DeltaTime) override;
	
	virtual void SetPawn(APawn* InPawn) override;
	void BindOnTankDeath(APawn * InPawn);
	UFUNCTION()
	void OnPossessedTankDeath();

	// Start moving the barrel so that a shot would hit where
	// the crosshair intersects the world
	void AimTowardsCrosshair();
	bool CanAim() const;
	bool GetSightRayHitLocation(FVector& OutHitLocation) const;
	bool GetLookDirection(FVector2D ScreenLocation, FVector& LookDirection) const;
	bool GetLookVectorHitLocation(FVector LookDirection, FVector& OutHitLocation) const;
	bool HasVisibleHitLocation(FHitResult& OutHitResult, const FVector& StartLocation, const FVector& EndLocation) const;

	// Crosshair X position ratio, relative to current viewport size
	UPROPERTY(EditDefaultsOnly)
	float CrosshairXScreenRelLocation = 0.5;
	// Crosshair Y position ratio, relative to current viewport size
	UPROPERTY(EditDefaultsOnly)
	float CrosshairYScreenRelLocation = 0.33333;

	UPROPERTY(EditDefaultsOnly)
	float LineTraceRange = 1000000;

	UTankAimingComponent* ControlledTankAimingComp;
};
