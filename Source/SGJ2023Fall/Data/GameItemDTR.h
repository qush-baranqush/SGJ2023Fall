#pragma once
#include "GameplayTagContainer.h"
#include "GameplayTagsManager.h"

#include "GameItemDTR.generated.h"

class AInteractableActor;
USTRUCT(BlueprintType)
struct FGameItemDTR : public FGameplayTagTableRow
{
	GENERATED_BODY()

public:
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly)
	FGameplayTag ItemTag;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly)
	FText Name;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly)
	UTexture2D* ItemIcon;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly)
	TSubclassOf<AInteractableActor> SpawnClass;	
};
