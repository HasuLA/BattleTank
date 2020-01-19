// Fill out your copyright notice in the Description page of Project Settings.

#include "TankMovementComponent.h"
#include "TankTrack.h"

void UTankMovementComponent::Initialise(UTankTrack* LeftTrackToSet, UTankTrack* RightTrackToSet)
{
	if (!LeftTrackToSet || !RightTrackToSet) { return;  }
	LeftTrack = LeftTrackToSet;
	RigthTrack = RightTrackToSet;
}

void UTankMovementComponent::IntendMovementForward(float Throw) 
{
	UE_LOG(LogTemp, Warning, TEXT("Intend move forward throw: %f"), Throw);

	LeftTrack->SetThrottle(Throw);
	RigthTrack->SetThrottle(Throw);
}


