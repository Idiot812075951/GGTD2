// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "UObject/NoExportTypes.h"
#include "MessageSystem.generated.h"

// 怪物被击杀消息的委托
DECLARE_DYNAMIC_MULTICAST_DELEGATE_TwoParams(FMonsterKilledDelegate, ACharacter*, Killer, ACharacter*, KilledMonster);
// 玩家击杀数量达到阈值消息的委托
DECLARE_DYNAMIC_MULTICAST_DELEGATE_TwoParams(FPlayerKillCountReachedDelegate, APlayerController*, Player, int32, KillCount);


/**
 * 
 */
UCLASS()
class GGTD2_API UMessageSystem : public UObject
{
	GENERATED_BODY()
public:
	// 单例模式获取消息系统实例
	UFUNCTION(BlueprintCallable, Category = "MessageSystem")
	static UMessageSystem* GetInstance();

	// 发送怪物被击杀的消息
	UFUNCTION(BlueprintCallable, Category = "MessageSystem")
	void SendMonsterKilledMessage(ACharacter* Killer, ACharacter* KilledMonster);

	// 发送玩家击杀数量达到阈值的消息
	UFUNCTION(BlueprintCallable, Category = "MessageSystem")
	void SendPlayerKillCountReachedMessage(APlayerController* Player, int32 KillCount);

	// 怪物被击杀消息的委托
	UPROPERTY(BlueprintAssignable, Category = "MessageSystem")
	FMonsterKilledDelegate OnMonsterKilled;

	// 玩家击杀数量达到阈值消息的委托
	UPROPERTY(BlueprintAssignable, Category = "MessageSystem")
	FPlayerKillCountReachedDelegate OnPlayerKillCountReached;

private:
	static UMessageSystem* Instance;
	
};
