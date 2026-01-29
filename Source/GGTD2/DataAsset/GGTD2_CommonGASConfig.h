// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameplayEffect.h"
#include "Engine/DataAsset.h"
#include "GGTD2_CommonGASConfig.generated.h"

/**
 * 
 */
UCLASS()
class GGTD2_API UGGTD2_CommonGASConfig : public UDataAsset
{
	GENERATED_BODY()
	
public:
	UFUNCTION()
	TSubclassOf<UGameplayEffect> GetEffectByName(FName Name)
	{
		if (auto Effect=GlobalEffectMap.Find(Name))
		{
			return *Effect;
		}
		return nullptr;
	};
	
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "DataAsset")
	TMap<FName,TSubclassOf<UGameplayEffect>> GlobalEffectMap;
	
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "DataAsset")
	TMap<FName,FGameplayTag> GlobalAssetTag;
};
