// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameplayEffectExecutionCalculation.h"
#include "GGTD_GEECBase.generated.h"

class AGGTD2_CharacterBase;
/**
 * 
 */
UCLASS()
class GGTD2_API UGGTD_GEECBase : public UGameplayEffectExecutionCalculation
{
	GENERATED_BODY()
public:
	//记录的伤害
	mutable float RecordDamage=0.f;
	//记录的GA
	mutable TSubclassOf<UGameplayAbility> RecordAbilityClass;
	//记录的施法者
	UPROPERTY()
	mutable AGGTD2_CharacterBase* RecordPlayerPawn;
};
