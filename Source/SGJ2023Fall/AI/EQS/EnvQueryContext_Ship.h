// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "EnvironmentQuery/EnvQueryContext.h"
#include "UObject/Object.h"
#include "EnvQueryContext_Ship.generated.h"

/**
 * 
 */
UCLASS()
class SGJ2023FALL_API UEnvQueryContext_Ship : public UEnvQueryContext
{
	GENERATED_BODY()

public:
	virtual void ProvideContext(FEnvQueryInstance& QueryInstance, FEnvQueryContextData& ContextData) const override;
};
