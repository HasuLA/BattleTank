// Copyright EmbraceIT Ltd.

#include "TankMovementComponent.h"
#include "TankTrack.h"

void UTankMovementComponent::Initialise(UTankTrack* LeftTrackToSet, UTankTrack* RightTrackToSet)
{
	LeftTrack = LeftTrackToSet;
	RightTrack = RightTrackToSet;
}

void UTankMovementComponent::RequestDirectMove(const FVector& MoveVelocity, bool bForceMaxSpeed)
{
	// No need to call Super as the functionality in intentionally replaced

	FVector NormalizedTankForwardVec = GetOwner()->GetActorForwardVector().GetSafeNormal();
	FVector NormalizedMovementVelocityVec = MoveVelocity.GetSafeNormal();

	float ForwardThrow = FVector::DotProduct(NormalizedTankForwardVec, NormalizedMovementVelocityVec);
	IntendMovementForward(ForwardThrow);

	float RigthThrow = FVector::CrossProduct(NormalizedTankForwardVec, NormalizedMovementVelocityVec).Z;
	IntendTurnRight(RigthThrow);
}

void UTankMovementComponent::IntendMovementForward(float Throw) 
{
	if (LeftTrack && RightTrack) 
	{
		LeftTrack->SetThrottle(Throw);
		RightTrack->SetThrottle(Throw);
	}
}

void UTankMovementComponent::IntendTurnRight(float Throw)
{
	if (LeftTrack && RightTrack) 
	{
		LeftTrack->SetThrottle(Throw);
		RightTrack->SetThrottle(-Throw);
	}
}


