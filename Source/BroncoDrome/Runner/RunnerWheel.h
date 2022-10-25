// // Copyright (C) Dromies 2021. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "ChaosVehicleWheel.h"
#include "RunnerWheel.generated.h"

/**
 * 
 */
UCLASS()
class BRONCODROME_API URunnerWheel : public UChaosVehicleWheel
{
	GENERATED_BODY()

public:		// Constructors

	URunnerWheel();

	//UPROPERTY(BlueprintReadWrite)
	int runnerSelected;

	UFUNCTION(BlueprintCallable, BlueprintNativeEvent, Category = URunnerWheel)
        void SelectRunner(int i);
};
