// 


#include "HouseLightsManagerComponent.h"

#include "GameFramework/GameplayMessageSubsystem.h"
#include "SGJ2023Fall/Settings/GameplaySettings.h"

// Called when the game starts
void UHouseLightsManagerComponent::BeginPlay()
{
	Super::BeginPlay();
	auto GameplaySettings = GetDefault<UGameplaySettings>();
	if (GameplaySettings->DarkTimeTag.IsValid())
	{
		UGameplayMessageSubsystem::Get(GetWorld()).RegisterListener(GameplaySettings->DarkTimeTag, this, &UHouseLightsManagerComponent::OnDarkTime);
	}
}

void UHouseLightsManagerComponent::OnDarkTime(FGameplayTag MessageTag, const FGameplayMessage_EmptyMessage& Message)
{
	OnDarkTimeHandle();
}
