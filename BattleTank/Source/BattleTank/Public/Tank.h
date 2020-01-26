// Copyright EmbraceIT Ltd.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Pawn.h"
#include "Tank.generated.h"

class UTankBarrel;
class UTankTurret;
class UTankAimingComponent;
class UTankMovementComponent;
class UWorld;
class AProjectile;


UCLASS()
class BATTLETANK_API ATank : public APawn
{
	GENERATED_BODY()

public:
	// Sets default values for this pawn's properties
	ATank();


	void AimAt(FVector HitLocation);

	UFUNCTION(BlueprintCallable, Category = Firing)
	void Fire();

	

protected:
	

	UTankAimingComponent* TankAimingComponent = nullptr; 

	UPROPERTY(BlueprintReadOnly)
	UTankMovementComponent* TankMovementComponent = nullptr;

private:
	UFUNCTION()
		void SomePrivateFunc()
	{
		UE_LOG(LogTemp, Warning, TEXT("SomePrivateFunc Log"))
	}

	UPROPERTY(EditDefaultsOnly, Category = Setup)
		TSubclassOf<AProjectile> ProjectileBlueprint;

	UPROPERTY(EditDefaultsOnly, Category = Firing)
		float LaunchSpeed = 4000; 

	UPROPERTY(EditDefaultsOnly, Category = Firing)
		float ReloadTimeInSeconds = 3;	

	// Local barrel reference for spawning projectile
	UTankBarrel* Barrel = nullptr; // TODO Remove

	double LastFireTime = 0;
};
