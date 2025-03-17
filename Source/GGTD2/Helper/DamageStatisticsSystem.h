// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "UObject/NoExportTypes.h"
#include "DamageStatisticsSystem.generated.h"

class UGameplayAbility;

// 伤害记录结构体
USTRUCT(BlueprintType)
struct FDamageRecord
{
	GENERATED_BODY()

	// 技能GA类的名称
	UPROPERTY(BlueprintReadOnly, Category = "DamageRecord")
	FName AbilityClassName;

	// 施放技能的玩家Pawn
	UPROPERTY(BlueprintReadOnly, Category = "DamageRecord")
	ACharacter* PlayerPawn;

	// 造成的伤害值
	UPROPERTY(BlueprintReadOnly, Category = "DamageRecord")
	float Damage;
};

// 伤害记录数组包装结构体
USTRUCT(BlueprintType)
struct FDamageRecordArrayWrapper
{
	GENERATED_BODY()

	UPROPERTY(BlueprintReadOnly, Category = "DamageRecordArrayWrapper")
	TArray<FDamageRecord> Records;
	// 技能释放次数
	UPROPERTY(BlueprintReadOnly, Category = "DamageRecordArrayWrapper")
	int32 AbilityCastCount = 0;
};


// 玩家伤害统计结构体
USTRUCT(BlueprintType)
struct FPlayerDamageStatistics
{
	GENERATED_BODY()

	// 玩家Pawn
	UPROPERTY(BlueprintReadOnly, Category = "PlayerDamageStatistics")
	ACharacter* PlayerPawn;

	// 技能伤害统计映射，键为技能GA类的名称，值为伤害记录数组
	UPROPERTY(BlueprintReadOnly, Category = "PlayerDamageStatistics")
	TMap<FName,FDamageRecordArrayWrapper> AbilityDamageRecords;
};

/**
 * 
 */
UCLASS()
class GGTD2_API UDamageStatisticsSystem : public UObject
{
	GENERATED_BODY()
public:
	// 单例模式获取伤害统计系统实例
	UFUNCTION(BlueprintCallable, Category = "DamageStatisticsSystem")
	static UDamageStatisticsSystem* GetInstance();

	// 记录伤害信息，每次技能释放，只能调用1次，这里的伤害是总伤害
	UFUNCTION(BlueprintCallable, Category = "DamageStatisticsSystem")
	void RecordDamage(TSubclassOf<UGameplayAbility> AbilityClass, ACharacter* PlayerPawn, float Damage,int Count=1);

	// 广播伤害统计信息给玩家
	UFUNCTION(BlueprintCallable, Category = "DamageStatisticsSystem")
	void BroadcastDamageStatistics(ACharacter* PlayerPawn, bool bShowAverageDamage);

private:
	static UDamageStatisticsSystem* Instance;

	// 玩家伤害统计映射，键为玩家Pawn，值为玩家伤害统计结构体
	TMap<ACharacter*, FPlayerDamageStatistics> PlayerDamageStats;
};
