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

  // Note: 0 means the variable is effectively unitialized for any int
  UPROPERTY(BlueprintReadOnly)
  int gamemodeSelected; // 1 is standard, 2 is survival, 3 is freeplay

  UPROPERTY(BlueprintReadOnly)
  int levelSelected; // 1 is stadium day, 2 stadium night, 3 sub day, 4 sub night

  UPROPERTY(BlueprintReadOnly)
  int runnerSelected; // 1 is balanced, 2 is speed, 3 is traction

  UPROPERTY(BlueprintReadOnly)
  int difficultySelected; // 1 is easy, 2 is medium, 3 is hard

  UPROPERTY(BlueprintReadOnly)
  FName gamemodeSelectedDisplay; // 1 is standard, 2 is survival, 3 is freeplay

  UPROPERTY(BlueprintReadOnly)
  FName levelSelectedDisplay; // 1 is stadium day, 2 stadium night, 3 sub day, 4 sub night

  UPROPERTY(BlueprintReadOnly)
  FName runnerSelectedDisplay; // 1 is balanced, 2 is speed, 3 is traction

  UPROPERTY(BlueprintReadOnly)
  FName difficultySelectedDisplay; // 1 is easy, 2 is medium, 3 is hard

  UFUNCTION(BlueprintCallable, Category = UGamemodeSelectWidgetParent)
  void SetSelectedGamemode(int gamemodeIndex);

  UFUNCTION(BlueprintCallable, Category = UGamemodeSelectWidgetParent)
  void SetSelectedLevel(int levelIndex);

  UFUNCTION(BlueprintCallable, Category = UGamemodeSelectWidgetParent)
  void SetSelectedRunner(int runnerIndex);

  UFUNCTION(BlueprintCallable, Category = UGamemodeSelectWidgetParent)
  void SetSelectedDifficulty(int difficultyIndex);

  FName GetGamemodeFName();
  FName GetLevelFName();
  FName GetRunnerFName();
  FName GetDifficultyFName();

  UFUNCTION(BlueprintCallable, Category = UGamemodeSelectWidgetParent)
  void SetDisplayStrings();

  void SetGamemodeDisplayString();
  void SetLevelDisplayString();
  void SetRunnerDisplayString();
  void SetDifficultyDisplayString();

  UFUNCTION(BlueprintCallable, Category = UGamemodeSelectWidgetParent)
  void InitSave();

  UFUNCTION(BlueprintCallable, Category = UGamemodeSelectWidgetParent)
  void PlayLevel();

  UFUNCTION(BlueprintCallable, Category = UGamemodeSelectWidgetParent)
  void ReturnToMainMenu();

  UFUNCTION(BlueprintCallable, BlueprintNativeEvent, Category = UGamemodeSelectWidgetParent)
  void PlayFadeInAnimation();
  

};