// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Abilities/GameplayAbilityTargetActor_Radius.h"
#include "TargetActor_Radius_FireBall.generated.h"

/**
 * 
 */
UCLASS()
class GGTD2_API ATargetActor_Radius_FireBall : public AGameplayAbilityTargetActor_Radius
{
	GENERATED_BODY()

	ATargetActor_Radius_FireBall();

public:
	virtual void StartTargeting(UGameplayAbility* Ability) override;
	virtual void ConfirmTargetingAndContinue() override;
};
