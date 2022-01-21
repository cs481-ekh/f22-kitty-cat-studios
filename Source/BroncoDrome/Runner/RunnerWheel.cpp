// // Copyright (C) Dromies 2021. All Rights Reserved.


#include "RunnerWheel.h"

URunnerWheel::URunnerWheel()
{
	// All wheel drive, baby
	bAffectedByBrake = true;
	bAffectedByHandbrake = true;
	bAffectedByEngine = true;
	bTractionControlEnabled = true;

	// Other attributes
	WheelRadius = 15.f;
	WheelWidth = 30.f;
	LateralFrictionForceMultiplier = 8.f;
	SideSlipModifier = 1.f;
	SlipThreshold = 12.f;
	SkidThreshold = 12.f;
	SuspensionDampingRatio = 2.f;
	MaxSteerAngle = 22.5f;
	SpringRate = 750.f;
	SuspensionMaxRaise = 15.f;
	SuspensionMaxDrop = 15.f;
}