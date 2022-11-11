// // Copyright (C) Team Kitty Cat 2022. All Rights Reserved.

#pragma once

#include "Blueprint/UserWidget.h"
#include "CoreMinimal.h"
#include "Engine/Texture2D.h"
#include "GamemodeSelectWidgetParent.generated.h"

/**
 *
 */
UCLASS()
class BRONCODROME_API UGamemodeSelectWidgetParent : public UUserWidget {
  GENERATED_BODY()

protected:  // Overrides
  virtual void NativePreConstruct() override;
  virtual void NativeConstruct() override;

public:  // Constructors
  // Instead, NativePreConstruct() and NativeConstruct() overrides to init
  // variables
  UPROPERTY(BlueprintReadOnly)
  int levelSelected;

  UPROPERTY(BlueprintReadOnly)
  int runnerSelected;

  UPROPERTY(BlueprintReadOnly)
  int difficultySelected;

  UFUNCTION(BlueprintCallable, Category = UGamemodeSelectWidgetParent)
  void SetSelectedLevel(int levelIndex);

  UFUNCTION(BlueprintCallable, Category = UGamemodeSelectWidgetParent)
  void PlayLevel();

  UFUNCTION(BlueprintCallable, Category = UGamemodeSelectWidgetParent)
  void ReturnToMainMenu();

  UFUNCTION(BlueprintCallable, BlueprintNativeEvent, Category = UGamemodeSelectWidgetParent)
  void PlayFadeInAnimation();
  

};