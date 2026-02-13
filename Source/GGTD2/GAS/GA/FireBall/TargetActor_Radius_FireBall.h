// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Abilities/GameplayAbilityTargetActor_Radius.h"
#include "GGTD2/GAS/GA/GGTD2_GameplayAbilityBase.h"
#include "TargetActor_Radius_FireBall.generated.h"

/**
 * 
 */
UCLASS(Blueprintable)
class GGTD2_API ATargetActor_Radius_FireBall : public AGameplayAbilityTargetActor_Radius
{
	GENERATED_BODY()

	ATargetActor_Radius_FireBall();

public:
	virtual void StartTargeting(UGameplayAbility* Ability) override;
	virtual void ConfirmTargetingAndContinue() override;
	void SetTargetSelectMode(EGATargetSelection InTargetSelection)
	{
		TargetSelection=InTargetSelection;
	}
private:
	EGATargetSelection TargetSelection;
};
