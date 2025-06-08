// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Abilities/GameplayAbility.h"
#include "GGTD2/GGTD2_CharacterBase.h"
#include "GGTD2/DataAsset/GGTD2_FireDataAsset.h"
#include "GA_FireBall.generated.h"

class AProjectileActor;
/**
 * 
 */
UCLASS()
class GGTD2_API UGA_FireBall : public UGameplayAbility
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
	TSubclassOf<AProjectileActor> ProjectileActorClass;
	
	virtual void ActivateAbility(const FGameplayAbilitySpecHandle Handle, const FGameplayAbilityActorInfo* ActorInfo, const FGameplayAbilityActivationInfo ActivationInfo, const FGameplayEventData* TriggerEventData) override;

	UFUNCTION()
	void OnMontageComplete();
	
	UFUNCTION()
	void OnMontageCancelled();

	UFUNCTION()
	void OnTargetSelected(const FGameplayAbilityTargetDataHandle& TargetData);
	void SpawnFireballToTarget(AGGTD2_CharacterBase* Target);
	
};
