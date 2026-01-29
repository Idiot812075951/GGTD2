// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Abilities/GameplayAbility.h"
#include "GGTD2_GameplayAbilityBase.generated.h"

UENUM(BlueprintType)
enum class EGATargetSelection : uint8
{
	Closest,      				// 最近的目标
	Farthest,     				// 最远的目标
	HighestHP,		    		// 生命值百分比最高的目标
	LowestHP,		    		// 生命值百分比最低的目标
	HighestHPPercent,   		// 生命值百分比最高的目标
	LowestHPPercent,    		// 生命值百分比最低的目标
	Strongest,          		// 最强的目标(可定义强度标准)
	Weakest,            		// 最弱的目标
	Random,             		// 随机目标
	Self,               		// 可选添加:自身
	Other,              		// 可能有复杂的情况
	Max							// 防越界
};


/**
 * 
 */
UCLASS()
class GGTD2_API UGGTD2_GameplayAbilityBase : public UGameplayAbility
{
	GENERATED_BODY()

public:
	UPROPERTY(BlueprintReadWrite, EditAnywhere,Category="GABase")
	EGATargetSelection TargetSelection;

	UPROPERTY(BlueprintReadWrite, EditAnywhere,Category="GABase")
	int TargetSelectNum=1;
};
