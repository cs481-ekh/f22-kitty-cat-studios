// // Copyright (C) Dromies 2021. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "ChaosVehicleWheel.h"
#include "BroncoDrome/BroncoSaveGame.h"
#include "GameFramework/GameUserSettings.h"
#include "Kismet/GameplayStatics.h"
#include "GameFramework/Character.h"
#include "RunnerWheel.generated.h"


/*USTRUCT(BlueprintType)
struct FRunnerWheelSelect {
  GENERATED_BODY()

  UPROPERTY(EditAnywhere, BlueprintReadWrite)
  FName runnerSelected;

  FRunnerWheelSelect() {
    runnerSelected = FName(TEXT("balanced"));
  }

	  void setParams(FName rs) { 
	  runnerSelected = rs;

  }
};
*/
/**
 * 
 */
UCLASS()
class BRONCODROME_API URunnerWheel : public UChaosVehicleWheel
{
	GENERATED_BODY()

public:		// Constructors

	URunnerWheel();

	  UPROPERTY(EditAnywhere, BlueprintReadWrite)
        FName runnerSelected;

	//UFUNCTION(BlueprintCallable, BlueprintNativeEvent, Category = URunnerWheel)
      //  void SelectRunner(int i);
};
