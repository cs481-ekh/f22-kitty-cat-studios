#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "LoseWidget.generated.h"

UCLASS()
class BRONCODROME_API ULoseWidget: public UUserWidget
{
	GENERATED_BODY()

protected:	// Overrides

	virtual void NativePreConstruct() override;
	virtual void NativeConstruct() override;
  
};
