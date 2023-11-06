﻿// 


#include "HUDWidget.h"

#include "GameFramework/GameplayMessageSubsystem.h"
#include "SGJ2023Fall/Settings/GameplaySettings.h"

void UHUDWidget::NativeConstruct()
{
	Super::NativeConstruct();
	
	const UGameplaySettings* GameplaySettings = GetDefault<UGameplaySettings>();
	if (GameplaySettings->ItemAcquiredMessageTag.IsValid())
		UGameplayMessageSubsystem::Get(GetWorld()).RegisterListener(GameplaySettings->ItemAcquiredMessageTag, this, &UHUDWidget::OnItemAcquired);

	if (GameplaySettings->ShipRequestTag.IsValid())
		UGameplayMessageSubsystem::Get(GetWorld()).RegisterListener(GameplaySettings->ShipRequestTag, this, &UHUDWidget::OnShipRequest);
}

void UHUDWidget::OnItemAcquired(FGameplayTag MessageTag, const FGameplayMessage_ItemPickedUp& Message)
{
	OnItemAcquiredHandle(Message.ItemTag, Message.ItemsCount);
}

void UHUDWidget::OnShipRequest(FGameplayTag MessageTag, const FGameplayMessage_ShipItemRequest& Message)
{
	OnShipRequestHandle(Message.ItemTag, Message.ItemsCount);
}
