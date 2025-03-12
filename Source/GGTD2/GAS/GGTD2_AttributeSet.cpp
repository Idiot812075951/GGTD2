// Fill out your copyright notice in the Description page of Project Settings.


#include "GGTD2_AttributeSet.h"


void UGGTD2_AttributeSet::PostAttributeChange(const FGameplayAttribute& Attribute, float OldValue, float NewValue)
{
	Super::PostAttributeChange(Attribute, OldValue, NewValue);
	if (Attribute.AttributeName.Equals("Health"))
	{
		OnHealthChange.Broadcast(NewValue);
	}
	if (Attribute.AttributeName.Equals("MaxHealth"))
	{
		OnMaxHealthChange.Broadcast(NewValue);
	}
}
