// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "GGTD2/GamePlay/Character/GGTD2_CharacterBase.h"
#include "AttributeSetWidget.generated.h"


DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FOnAttributeChange, float, HealthChange);

/**
 * 
 */
UCLASS()
class GGTD2_API UAttributeSetWidget : public UUserWidget
{
	GENERATED_BODY()
public:

	UPROPERTY(BlueprintAssignable)
	FOnAttributeChange OnHealthChange;
	
	UPROPERTY(BlueprintAssignable)
	FOnAttributeChange OnMaxHealthChange;

	UPROPERTY(BlueprintReadOnly,VisibleAnywhere)
	float DeltaHealth{0};
	
	void BindCharacter(AGGTD2_CharacterBase* CharacterBase);

	virtual void NativeConstruct() override;
	
	void HandleHealthChange(const FOnAttributeChangeData& Health);
	void HandleMaxHealthChange(const FOnAttributeChangeData& MaxHealth) const;
	
};
