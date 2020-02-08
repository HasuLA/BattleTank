// Copyright EmbraceIT Ltd.

#include "TankAimingComponent.h"
#include "Engine/World.h"
#include "Kismet/GameplayStatics.h"
#include "TankBarrel.h"
#include "TankTurret.h"
#include "Projectile.h"
#include "Components/StaticMeshComponent.h"

UTankAimingComponent::UTankAimingComponent()
{
	PrimaryComponentTick.bCanEverTick = true;
	bWantsBeginPlay = true;
}

void UTankAimingComponent::BeginPlay()
{
	LastFireTime = FPlatformTime::Seconds();
}

void UTankAimingComponent::Initialise(UTankBarrel* BarrelToSet, UTankTurret* TurretToSet)
{
	Barrel = BarrelToSet;
	Turret = TurretToSet;
}

void UTankAimingComponent::TickComponent(float DeltaTime, enum ELevelTick TickType, FActorComponentTickFunction *ThisTickFunction)
{
	UpdateFiringState();
}

void UTankAimingComponent::UpdateFiringState()
{
	if (CurrentAmmo <= 0)
	{
		FiringState = EFiringState::OutOfAmmo;
	}
	else if ((FPlatformTime::Seconds() - LastFireTime) < ReloadTimeInSeconds)
	{
		FiringState = EFiringState::Reloading;
	}
	else if (IsBarrelMoving())
	{
		FiringState = EFiringState::Aiming;
	}
	else
	{
		FiringState = EFiringState::Locked;
	}
}

int32 UTankAimingComponent::GetCurrentAmmo() const
{
	return CurrentAmmo;
}

EFiringState UTankAimingComponent::GetFiringState() const
{
	return FiringState;
}

bool UTankAimingComponent::IsBarrelMoving()
{
	if (ensure(Barrel)) 
	{
		FVector BarrelForward = Barrel->GetForwardVector();
		return !BarrelForward.Equals(AimDirection, 0.01);
	}
	
	return false;
}

void UTankAimingComponent::AimAt(FVector HitLocation)
{
	if (ensure(Barrel)) 
	{ 
		FVector OutLaunchVelocity;
		FVector StartLocation = Barrel->GetSocketLocation(BarrelMuzzleSocketName);
		
		bool bHaveAimSolution = UGameplayStatics::SuggestProjectileVelocity
		(
			this,
			OutLaunchVelocity,
			StartLocation,
			HitLocation,
			ProjectileLaunchSpeed,
			false,
			0,
			0,
			ESuggestProjVelocityTraceOption::DoNotTrace // Paramater must be present to prevent bug
		);
		
		if (bHaveAimSolution)
		{
			AimDirection = OutLaunchVelocity.GetSafeNormal();
			MoveBarrelTowards(AimDirection);
		}
		// If no solution found do nothing
	}
}
void UTankAimingComponent::MoveBarrelTowards(FVector DesiredBarrelOrientation)
{
	if (ensure(Barrel) && ensure(Turret)) 
	{
		// Work-out difference between current barrel rotaion, and AimDirection
		FRotator BarrelCurrentRotation = Barrel->GetForwardVector().Rotation();
		FRotator DesiredBarrelRotation = DesiredBarrelOrientation.Rotation();
		FRotator DeltaRotation = DesiredBarrelRotation - BarrelCurrentRotation;
		Barrel->Elevate(DeltaRotation.Pitch);
		
		// Always Yaw the shortest way
		if (FMath::Abs(DeltaRotation.Yaw) < 180)
		{
			Turret->Rotate(DeltaRotation.Yaw);
		}
		else
		{
			Turret->Rotate(-DeltaRotation.Yaw);
		}
	}
}

void UTankAimingComponent::Fire()
{
	if (CanFire())
	{
		ExecuteShot();
	}
}

bool UTankAimingComponent::CanFire() const
{
	return FiringState == EFiringState::Locked || FiringState == EFiringState::Aiming;
}

void UTankAimingComponent::ExecuteShot()
{
	if (ensure(Barrel) && ensure(ProjectileBlueprint))
	{
		SpawnProjectile();
		RemberLastShotTime();
		SpentAmmo();
	}
}

void UTankAimingComponent::SpawnProjectile()
{
	AProjectile* Projectile = GetWorld()->SpawnActor<AProjectile>(
		ProjectileBlueprint,
		Barrel->GetSocketLocation(BarrelMuzzleSocketName),
		Barrel->GetSocketRotation(BarrelMuzzleSocketName)
		);
	Projectile->LaunchProjectile(ProjectileLaunchSpeed);
}

void UTankAimingComponent::RemberLastShotTime()
{
	LastFireTime = FPlatformTime::Seconds();
}

void UTankAimingComponent::SpentAmmo()
{
	CurrentAmmo--;
}

