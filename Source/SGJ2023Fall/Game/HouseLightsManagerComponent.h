// 

#pragma once

#include "CoreMinimal.h"
#include "GameplayTagContainer.h"
#include "Components/ActorComponent.h"
#include "SGJ2023Fall/Data/GameplayMessages.h"
#include "HouseLightsManagerComponent.generated.h"


UCLASS(Blueprintable, ClassGroup=(Custom), meta=(BlueprintSpawnableComponent))
class SGJ2023FALL_API UHouseLightsManagerComponent : public UActorComponent
{
	GENERATED_BODY()

protected:
	// Called when the game starts
	virtual void BeginPlay() override;

	UFUNCTION(BlueprintImplementableEvent)
	void OnDarkTimeHandle();
	
private:
	void OnDarkTime(FGameplayTag MessageTag, const FGameplayMessage_EmptyMessage& Message);
};
