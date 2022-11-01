// // Copyright (C) Team Kitty Cat 2022. All Rights Reserved.

#pragma once

#include "Blueprint/UserWidget.h"
#include "CoreMinimal.h"
#include "Engine/Texture2D.h"
#include "LevelSelectWidget.generated.h"

/**
 *
 */
UCLASS()
class BRONCODROME_API ULevelSelectWidget : public UUserWidget {
  GENERATED_BODY()

protected:  // Overrides
  virtual void NativePreConstruct() override;
  virtual void NativeConstruct() override;

public:  // Constructors
  // Instead, NativePreConstruct() and NativeConstruct() overrides to init
  // variables
  UPROPERTY(BlueprintReadOnly)
  int levelSelected;

  UFUNCTION(BlueprintCallable, Category = ULevelSelectWidget)
  void SetSelectedLevel(int levelIndex);

};