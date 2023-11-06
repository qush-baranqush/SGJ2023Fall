// 

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "SGJ2023Fall/Data/GameplayMessages.h"
#include "GameplayTagContainer.h"
#include "HUDWidget.generated.h"

/**
 * 
 */
UCLASS()
class SGJ2023FALL_API UHUDWidget : public UUserWidget
{
	GENERATED_BODY()

public:
	virtual void NativeConstruct() override;

protected:
	UFUNCTION(BlueprintImplementableEvent)
	void OnItemAcquiredHandle(FGameplayTag ItemTag, int Count);

	UFUNCTION(BlueprintImplementableEvent)
	void OnShipRequestHandle(FGameplayTag ItemTag, int Count);
	
private:
	void OnItemAcquired(FGameplayTag MessageTag, const FGameplayMessage_ItemPickedUp& Message);
	void OnShipRequest(FGameplayTag MessageTag, const FGameplayMessage_ShipItemRequest& Message);
};
