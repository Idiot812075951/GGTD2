// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameplayEffectExecutionCalculation.h"
#include "GGTD_GEECBase.h"
#include "FireDamageExecution.generated.h"

/**
 * 
 */
UCLASS()
class GGTD2_API UFireDamageExecution : public UGGTD_GEECBase
{
	GENERATED_BODY()

public:
	UFireDamageExecution();
	FGameplayEffectAttributeCaptureDefinition ArmorDef;
	FGameplayEffectAttributeCaptureDefinition HealthDef;

	virtual void Execute_Implementation(const FGameplayEffectCustomExecutionParameters& ExecutionParams, FGameplayEffectCustomExecutionOutput& OutExecutionOutput) const override;

};
