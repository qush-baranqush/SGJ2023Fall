#pragma once
#include "GameplayTagContainer.h"

#include "ShipData.generated.h"


USTRUCT(BlueprintType)
struct FShipRequest
{
	GENERATED_BODY()
	
	UPROPERTY(EditAnywhere, BlueprintReadOnly)
	FGameplayTag ItemTag;

	UPROPERTY(EditAnywhere, BlueprintReadOnly)
	int CountRequired = 1;
	
	int CurrentCount = 0;
};

USTRUCT(BlueprintType)
struct FShipEvent
{
	GENERATED_BODY()

	UPROPERTY(EditAnywhere, BlueprintReadOnly)
	TArray<FShipRequest> ItemsRequired;

	UPROPERTY(EditAnywhere, BlueprintReadOnly)
	float TimeDelay = 10.f;

	UPROPERTY(EditAnywhere, BlueprintReadOnly)
	UStaticMesh* ShipStageStaticMesh;
	
	UPROPERTY(EditAnywhere, BlueprintReadOnly)
	float ProgressGranted = 0.f;
};
