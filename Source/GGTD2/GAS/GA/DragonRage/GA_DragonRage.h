// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Abilities/GameplayAbility.h"
#include "GGTD2/GAS/GA/GGTD2_GameplayAbilityBase.h"
#include "GA_DragonRage.generated.h"

class UAbilityTask_WaitGameplayEvent;
class AGGTD2_CharacterBase;
/**
 * 
 */
UCLASS()
class GGTD2_API UGA_DragonRage : public UGGTD2_GameplayAbilityBase
{
	GENERATED_BODY()

public:
	UPROPERTY(BlueprintReadWrite,EditAnywhere)
	UAnimMontage* Montage_Cast;

	UPROPERTY(BlueprintReadWrite,EditAnywhere)
	FGameplayTag AniNotifyTag;
	
	UPROPERTY(BlueprintReadWrite,EditAnywhere)
	FVector ImpulseOffset{(0,0,1000)};
	
	UPROPERTY(BlueprintReadWrite,EditAnywhere)
	int ImpulseScale{500};

	virtual void ActivateAbility(const FGameplayAbilitySpecHandle Handle, const FGameplayAbilityActorInfo* ActorInfo, const FGameplayAbilityActivationInfo ActivationInfo, const FGameplayEventData* TriggerEventData) override;
	UFUNCTION()
	void OnMontageNotifyBegin( FGameplayEventData Payload);
	UFUNCTION()
	void OnMontageCancelled();
	
	UFUNCTION()
	void SpawnTargetActor();

	UFUNCTION()
	void OnTargetSelected(const FGameplayAbilityTargetDataHandle& TargetData);
	void DoWork();
private:
	AGGTD2_CharacterBase* TargetCharacter;
	AGGTD2_CharacterBase* OwnerCharacter;
	//每次释放技能的蒙太奇仍无
	UAbilityTask_WaitGameplayEvent* NotifyTask;
};
