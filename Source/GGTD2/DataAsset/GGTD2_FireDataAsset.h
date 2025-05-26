// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameplayEffect.h"
#include "GameplayTagContainer.h"
#include "GGTD2_ElementDataAsset.h"
#include "GGTD2_FireDataAsset.generated.h"

/**
 * 
 */
UCLASS()
class GGTD2_API UGGTD2_FireDataAsset : public UGGTD2_ElementDataAsset
{
	GENERATED_BODY()

public:
	// 基础伤害
	UPROPERTY(EditAnywhere,BlueprintReadWrite, Category="Fire")
	float BaseDamage = 50.0f;

	// 爆炸概率（0-1）
	UPROPERTY(EditAnywhere,BlueprintReadWrite, Category="Fire")
	float ExplosionChance = 0.3f;

	// 爆炸半径
	UPROPERTY(EditAnywhere,BlueprintReadWrite, Category="Fire")
	float ExplosionRadius = 500.0f;

	// 爆炸伤害比例（基于初始伤害）
	UPROPERTY(EditAnywhere,BlueprintReadWrite, Category="Fire")
	float ExplosionDamageRatio = 0.5f;

	UPROPERTY(EditAnywhere,BlueprintReadWrite, Category="Fire")
	bool CanBurning=true;
	
	//点燃百分比
	UPROPERTY(EditAnywhere,BlueprintReadWrite, Category="Fire")
	float BurningPercent=0.01f;	
	//点燃持续几秒
	UPROPERTY(EditAnywhere,BlueprintReadWrite, Category="Fire")
	float BurningSeconds=5.0f;

	// 对应GameplayTag
	UPROPERTY(EditAnywhere,BlueprintReadWrite, Category="Fire")
	FGameplayTag ElementTag = FGameplayTag::RequestGameplayTag("Gameplay.Element.Fire");
	
	// 对应GameplayTag
	UPROPERTY(EditAnywhere,BlueprintReadWrite, Category="Fire")
	FGameplayTag BurningCueTag = FGameplayTag::RequestGameplayTag("GameplayCue.Burning");

	UPROPERTY(EditAnywhere, BlueprintReadWrite,Category="Fire")
	TSubclassOf<UGameplayEffect> GE_Burning;
	
};
