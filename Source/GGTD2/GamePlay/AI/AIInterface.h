// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "UObject/Interface.h"
#include "AIInterface.generated.h"

// 声明AI分类枚举（后续扩展用）
UENUM(BlueprintType)
enum class EAICharacterCategory : uint8
{
	ECC_Grunt,    // 杂兵
	ECC_Elite,    // 精英
	ECC_Boss,     // BOSS
	ECC_Ally      // 盟友
};

// 智能度枚举（影响闪避/格挡概率）
UENUM(BlueprintType)
enum class EAIIntelligenceLevel : uint8
{
	EIL_Low,      // 杂兵：低智能
	EIL_Medium,   // 精英：中智能
	EIL_High      // BOSS：高智能
};

// This class does not need to be modified.
UINTERFACE(MinimalAPI)
class UAIInterface : public UInterface
{
	GENERATED_BODY()
};

/**
 * 
 */
class GGTD2_API IAIInterface
{
	GENERATED_BODY()

	// Add interface functions to this class. This is the class that will be inherited to implement this interface.
public:
};
