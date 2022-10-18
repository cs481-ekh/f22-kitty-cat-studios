// // Copyright (C) Team Kitty Cat 2022. All Rights Reserved.

#pragma once

#include "Blueprint/UserWidget.h"
#include "CoreMinimal.h"
#include "Engine/Texture2D.h"
#include "PowerupWidget.generated.h"

/**
 *
 */
UCLASS()
class BRONCODROME_API UPowerupWidget : public UUserWidget {
  GENERATED_BODY()

public:  // Constructors
  // Instead, NativePreConstruct() and NativeConstruct() overrides to init
  // variables
  UPROPERTY(BlueprintReadOnly)
  FString powerupName;

protected:  // Overrides
  virtual void NativePreConstruct() override;
  virtual void NativeConstruct() override;

private:  // Members
public:   // Interface
  UFUNCTION(BlueprintCallable, Category = UPowerupWidget)
  void SetPowerupText(FString powerupString);

  // Function to return the current player score
  UFUNCTION(BlueprintCallable, Category = UPowerupWidget)
  FString GetPowerupText();

  UFUNCTION(BlueprintCallable, BlueprintNativeEvent, Category = UPowerupWidget)
  void PlayFadeInAnimation();
};
