// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Abilities/GameplayAbility.h"
#include "GGTD2/DataAsset/GGTD2_FireDataAsset.h"
#include "GGTD2/GamePlay/Character/GGTD2_CharacterBase.h"
#include "GGTD2/GAS/GA/GGTD2_GameplayAbilityBase.h"
#include "GA_FireBall.generated.h"

class AProjectileActor;
/**
 * 
 */
UCLASS(Blueprintable, BlueprintType)
class GGTD2_API UGA_FireBall : public UGGTD2_GameplayAbilityBase
{
	GENERATED_BODY()

public:
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	float FireBallDamage=10.f;
	
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	UGGTD2_FireDataAsset* FireData;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	TSubclassOf<UGameplayEffect> GE_ElementDamage;
	
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	TSubclassOf<UGameplayEffect> GE_GADamage;
	
	UPROPERTY(BlueprintReadWrite,EditAnywhere)
	UAnimMontage* Montage_Cast;

	UPROPERTY(BlueprintReadWrite,EditAnywhere)
	FGameplayTag AniNotifyTag;
	
	UPROPERTY(BlueprintReadWrite,EditAnywhere)
	TSubclassOf<AProjectileActor> ProjectileActorClass;
	
	virtual void ActivateAbility(const FGameplayAbilitySpecHandle Handle, const FGameplayAbilityActorInfo* ActorInfo, const FGameplayAbilityActivationInfo ActivationInfo, const FGameplayEventData* TriggerEventData) override;

	UFUNCTION()
	void SpawnTargetActor();
	
	UFUNCTION()
	void OnMontageCancelled();

	UFUNCTION()
	void OnTargetSelected(const FGameplayAbilityTargetDataHandle& TargetData);
	UFUNCTION()
	void SpawnFireballToTarget(AGGTD2_CharacterBase* Target);

	UFUNCTION()
	void OnMontageNotifyBegin( FGameplayEventData Payload);
	UFUNCTION()
	void OnMontageOnInterrupted(FName NotifyName);
};
