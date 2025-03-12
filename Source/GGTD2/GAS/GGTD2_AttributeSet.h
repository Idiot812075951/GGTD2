// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "AttributeSet.h"
#include "AbilitySystemComponent.h"
#include "GGTD2_AttributeSet.generated.h"

#define ATTRIBUTE_ACCESSORS(ClassName, PropertyName) \
GAMEPLAYATTRIBUTE_PROPERTY_GETTER(ClassName, PropertyName) \
GAMEPLAYATTRIBUTE_VALUE_GETTER(PropertyName) \
GAMEPLAYATTRIBUTE_VALUE_SETTER(PropertyName) \
GAMEPLAYATTRIBUTE_VALUE_INITTER(PropertyName)


DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FOnAttributeChange, float, HealthChange);


/**
 * 
 */
UCLASS()
class GGTD2_API UGGTD2_AttributeSet : public UAttributeSet
{
	GENERATED_BODY()

public:
	UPROPERTY(BlueprintAssignable)
	FOnAttributeChange OnHealthChange;
	
	UPROPERTY(BlueprintAssignable)
	FOnAttributeChange OnMaxHealthChange;
	
	virtual void PostAttributeChange(const FGameplayAttribute& Attribute, float OldValue, float NewValue) override;

	UPROPERTY(VisibleAnywhere, BlueprintReadWrite);
	FGameplayAttributeData Health;
	ATTRIBUTE_ACCESSORS(UGGTD2_AttributeSet, Health);

	UPROPERTY(VisibleAnywhere, BlueprintReadWrite);
	FGameplayAttributeData MaxHealth;
	ATTRIBUTE_ACCESSORS(UGGTD2_AttributeSet, MaxHealth);
	
	UPROPERTY(VisibleAnywhere, BlueprintReadWrite);
	FGameplayAttributeData Armor;
	ATTRIBUTE_ACCESSORS(UGGTD2_AttributeSet, Armor);

	UPROPERTY(VisibleAnywhere, BlueprintReadWrite);
	FGameplayAttributeData MaxArmor;
	ATTRIBUTE_ACCESSORS(UGGTD2_AttributeSet, MaxArmor);
	
	
};
