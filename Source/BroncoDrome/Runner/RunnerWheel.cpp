// // Copyright (C) Dromies 2021. All Rights Reserved.


#include "RunnerWheel.h"

URunnerWheel::URunnerWheel()
{
	// All wheel drive, baby
	bAffectedByBrake = true;
	bAffectedByHandbrake = true;
	bAffectedByEngine = true;
	bTractionControlEnabled = true;

	enum RunnerStats{Speed, Traction, Balanced};
	RunnerStats runnerSelected = Balanced;

	Switch(runnerSelected)
	{
		case(Speed):
			WheelRadius = 10.f;
			WheelWidth = 30.f;
			LateralFrictionForceMultiplier = 4.f;
			SideSlipModifier = 1.f;
			SlipThreshold = 8.f;
			SkidThreshold = 8.f;
			SuspensionDampingRatio = 1.f;
			MaxSteerAngle = 27.5f;
			SpringRate = 550.f;
			SuspensionMaxRaise = 10.f;
			SuspensionMaxDrop = 10.f;
			break;
		case(Traction):
			WheelRadius = 20.f;
			WheelWidth = 35.f;
			LateralFrictionForceMultiplier = 10.f;
			SideSlipModifier = 1.f;
			SlipThreshold = 15.f;
			SkidThreshold = 15.f;
			SuspensionDampingRatio = 5.f;
			MaxSteerAngle = 17.5f;
			SpringRate = 1000.f;
			SuspensionMaxRaise = 20.f;
			SuspensionMaxDrop = 20.f;
			break;
		case(Balanced):
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
			break;
		Default:
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
			break;
	}

}
