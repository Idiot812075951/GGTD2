// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GGTD_GEECBase.h"
#include "InstallKillExecution.generated.h"

/**
 * 
 */
UCLASS()
class GGTD2_API UInstallKillExecution : public UGGTD_GEECBase
{
	GENERATED_BODY()
	
	virtual void Execute_Implementation(const FGameplayEffectCustomExecutionParameters& ExecutionParams, FGameplayEffectCustomExecutionOutput& OutExecutionOutput) const override;

};
