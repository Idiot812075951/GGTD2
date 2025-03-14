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



/**
 * 
 */
UCLASS()
class GGTD2_API UGGTD2_AttributeSet : public UAttributeSet
{
	GENERATED_BODY()

public:

	virtual void PostAttributeChange(const FGameplayAttribute& Attribute, float OldValue, float NewValue) override;

	virtual void GetLifetimeReplicatedProps(TArray<class FLifetimeProperty>& OutLifetimeProps) const override;

	UPROPERTY(VisibleAnywhere, BlueprintReadWrite,ReplicatedUsing=OnRep_Health)
	FGameplayAttributeData Health;
	ATTRIBUTE_ACCESSORS(UGGTD2_AttributeSet, Health);
	UFUNCTION()
	void OnRep_Health(const FGameplayAttributeData& OldHealth) const;

	UPROPERTY(VisibleAnywhere, BlueprintReadWrite,ReplicatedUsing=OnRep_MaxHealth);
	FGameplayAttributeData MaxHealth;
	ATTRIBUTE_ACCESSORS(UGGTD2_AttributeSet, MaxHealth);
	UFUNCTION()
	void OnRep_MaxHealth(const FGameplayAttributeData& OldHealth) const;
	
	UPROPERTY(VisibleAnywhere, BlueprintReadWrite);
	FGameplayAttributeData Armor;
	ATTRIBUTE_ACCESSORS(UGGTD2_AttributeSet, Armor);

	UPROPERTY(VisibleAnywhere, BlueprintReadWrite);
	FGameplayAttributeData MaxArmor;
	ATTRIBUTE_ACCESSORS(UGGTD2_AttributeSet, MaxArmor);
	
};



