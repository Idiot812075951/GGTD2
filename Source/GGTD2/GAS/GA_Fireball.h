// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Abilities/GameplayAbility.h"
#include "GGTD2/DataAsset/GGTD2_FireDataAsset.h"
#include "GA_Fireball.generated.h"

/**
 * 
 */
UCLASS()
class GGTD2_API UGA_Fireball : public UGameplayAbility
{
	GENERATED_BODY()
public:
	UPROPERTY(EditAnywhere, BlueprintReadWrite,Category="Fire")
	UGGTD2_FireDataAsset* FireData;

	virtual void ActivateAbility(const FGameplayAbilitySpecHandle Handle, const FGameplayAbilityActorInfo* ActorInfo, const FGameplayAbilityActivationInfo ActivationInfo, const FGameplayEventData* TriggerEventData) override;
	UPROPERTY(EditAnywhere, BlueprintReadWrite,Category="Fire")
	TSubclassOf<UGameplayEffect> GE_FireDamage;

};
