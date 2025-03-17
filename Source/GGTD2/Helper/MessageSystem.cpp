// Fill out your copyright notice in the Description page of Project Settings.


#include "MessageSystem.h"

UMessageSystem* UMessageSystem::Instance = nullptr;

UMessageSystem* UMessageSystem::GetInstance()
{
	if (!Instance)
	{
		Instance = NewObject<UMessageSystem>();
		Instance->AddToRoot();
	}
	return Instance;
}

void UMessageSystem::SendMonsterKilledMessage(ACharacter* Killer, ACharacter* KilledMonster)
{
	OnMonsterKilled.Broadcast(Killer, KilledMonster);
}

void UMessageSystem::SendPlayerKillCountReachedMessage(APlayerController* Player, int32 KillCount)
{
	OnPlayerKillCountReached.Broadcast(Player, KillCount);
}    
