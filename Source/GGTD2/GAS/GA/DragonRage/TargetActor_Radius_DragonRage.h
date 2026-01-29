// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Abilities/GameplayAbilityTargetActor_Radius.h"
#include "GGTD2/GAS/GA/GGTD2_GameplayAbilityBase.h"
#include "TargetActor_Radius_DragonRage.generated.h"

/**
 * 
 */
UCLASS()
class GGTD2_API ATargetActor_Radius_DragonRage : public AGameplayAbilityTargetActor_Radius
{
	GENERATED_BODY()

public:
	void SetTargetSelectMode(EGATargetSelection InTargetSelection)
	{
		TargetSelection=InTargetSelection;
	}
	void SetRadius(float InRadius);
	
	virtual void StartTargeting(UGameplayAbility* Ability) override;
	virtual void ConfirmTargetingAndContinue() override;
private:
	EGATargetSelection TargetSelection;
};
