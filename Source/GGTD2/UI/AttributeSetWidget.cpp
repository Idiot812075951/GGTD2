// Fill out your copyright notice in the Description page of Project Settings.


#include "AttributeSetWidget.h"

#include "GGTD2/GAS/GGTD2_AttributeSet.h"

void UAttributeSetWidget::BindCharacter(AGGTD2_CharacterBase* CharacterBase)
{
	FString Name = CharacterBase->GetName();
	CharacterBase->Asc->GetGameplayAttributeValueChangeDelegate(UGGTD2_AttributeSet::GetHealthAttribute())
	.AddUObject(this, &UAttributeSetWidget::HandleHealthChange);
	CharacterBase->Asc->GetGameplayAttributeValueChangeDelegate(UGGTD2_AttributeSet::GetMaxHealthAttribute())
		.AddUObject(this, &UAttributeSetWidget::HandleMaxHealthChange);
}

void UAttributeSetWidget::NativeConstruct()
{
	Super::NativeConstruct();
}

void UAttributeSetWidget::HandleHealthChange(const FOnAttributeChangeData& Health) const
{
	OnHealthChange.Broadcast(Health.NewValue);
}

void UAttributeSetWidget::HandleMaxHealthChange(const FOnAttributeChangeData& MaxHealth) const
{
	OnMaxHealthChange.Broadcast(MaxHealth.NewValue);
}
