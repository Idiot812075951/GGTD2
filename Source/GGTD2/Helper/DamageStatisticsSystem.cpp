// Fill out your copyright notice in the Description page of Project Settings.


#include "DamageStatisticsSystem.h"

#include "MessageSystem.h"
#include "Abilities/GameplayAbility.h"
#include "GameFramework/Character.h"

UDamageStatisticsSystem* UDamageStatisticsSystem::Instance = nullptr;

UDamageStatisticsSystem* UDamageStatisticsSystem::GetInstance()
{
    if (!Instance)
    {
        Instance = NewObject<UDamageStatisticsSystem>();
        Instance->AddToRoot();
    }
    return Instance;
}

void UDamageStatisticsSystem::RecordDamage(TSubclassOf<UGameplayAbility> AbilityClass, ACharacter* PlayerPawn, float Damage,int Count)
{
    if (!PlayerPawn) return;

    // 查找或创建玩家的伤害统计信息
    FPlayerDamageStatistics& PlayerStats = PlayerDamageStats.FindOrAdd(PlayerPawn);
    PlayerStats.PlayerPawn = PlayerPawn;

    FName AbilityClassName = AbilityClass ? AbilityClass->GetFName() : NAME_None;

    // 查找或创建技能的伤害记录数组包装结构体
    FDamageRecordArrayWrapper& AbilityRecordWrapper = PlayerStats.AbilityDamageRecords.FindOrAdd(AbilityClassName);

    // 添加新的伤害记录
    FDamageRecord NewRecord;
    NewRecord.AbilityClassName = AbilityClassName;
    NewRecord.PlayerPawn = PlayerPawn;
    NewRecord.Damage = Damage;
    AbilityRecordWrapper.Records.Add(NewRecord);
    
    // 增加技能释放次数
    AbilityRecordWrapper.AbilityCastCount+=Count;
}

void UDamageStatisticsSystem::BroadcastDamageStatistics(ACharacter* PlayerPawn, bool bShowAverageDamage)
{
    if (!PlayerPawn) return;

    FString Message = FString::Printf(TEXT("玩家 %s 的伤害统计信息：\n"), *PlayerPawn->GetName());

    // 获取玩家的伤害统计信息
    if (PlayerDamageStats.Contains(PlayerPawn))
    {
        const FPlayerDamageStatistics& PlayerStats = PlayerDamageStats[PlayerPawn];

        // 对技能伤害统计进行排序
        TArray<FName> SortedAbilityClassNames;
        PlayerStats.AbilityDamageRecords.GetKeys(SortedAbilityClassNames);
        SortedAbilityClassNames.Sort([&](const FName& A, const FName& B)
        {
            const FDamageRecordArrayWrapper& WrapperA = PlayerStats.AbilityDamageRecords[A];
            const FDamageRecordArrayWrapper& WrapperB = PlayerStats.AbilityDamageRecords[B];

            const TArray<FDamageRecord>& RecordsA = WrapperA.Records;
            const TArray<FDamageRecord>& RecordsB = WrapperB.Records;

            float TotalDamageA = 0.0f;
            for (const auto& Record : RecordsA)
            {
                TotalDamageA += Record.Damage;
            }
            float AverageDamageA = WrapperA.AbilityCastCount > 0 ? TotalDamageA / WrapperA.AbilityCastCount : 0.0f;

            float TotalDamageB = 0.0f;
            for (const auto& Record : RecordsB)
            {
                TotalDamageB += Record.Damage;
            }
            float AverageDamageB = WrapperB.AbilityCastCount > 0 ? TotalDamageB / WrapperB.AbilityCastCount : 0.0f;

            if (bShowAverageDamage)
            {
                return AverageDamageA > AverageDamageB;
            }
            else
            {
                return TotalDamageA > TotalDamageB;
            }
        });

        // 生成消息内容
        for (const auto& AbilityClassName : SortedAbilityClassNames)
        {
            const FDamageRecordArrayWrapper& Wrapper = PlayerStats.AbilityDamageRecords[AbilityClassName];
            const TArray<FDamageRecord>& Records = Wrapper.Records;
            float TotalDamage = 0.0f;
            for (const auto& Record : Records)
            {
                TotalDamage += Record.Damage;
            }
            float AverageDamage = Wrapper.AbilityCastCount > 0 ? TotalDamage / Wrapper.AbilityCastCount : 0.0f;

            if (bShowAverageDamage)
            {
                Message += FString::Printf(TEXT("技能 %s 平均伤害：%.2f（释放次数：%d）\n"), *AbilityClassName.ToString(), AverageDamage, Wrapper.AbilityCastCount);
            }
            else
            {
                Message += FString::Printf(TEXT("技能 %s 总伤害：%.2f（释放次数：%d）\n"), *AbilityClassName.ToString(), TotalDamage, Wrapper.AbilityCastCount);
            }
        }
    }
    
    GEngine->AddOnScreenDebugMessage(-1, 6.0f, FColor::Red, Message);

    // 使用消息系统广播消息
    //UMessageSystem::GetInstance()->SendMessageToPlayer(PlayerPawn, Message);
}    