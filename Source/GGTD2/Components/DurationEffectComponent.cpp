// Fill out your copyright notice in the Description page of Project Settings.


#include "DurationEffectComponent.h"

#include "GameplayTagContainer.h"
#include "GGTD2/GGTD2Character.h"

// Sets default values for this component's properties
UDurationEffectComponent::UDurationEffectComponent()
{
	// Set this component to be initialized when the game starts, and to be ticked every frame.  You can turn these features
	// off to improve performance if you don't need them.
	PrimaryComponentTick.bCanEverTick = true;
	OwnerAsc=nullptr;
	// ...
}


// Called when the game starts
void UDurationEffectComponent::BeginPlay()
{
	Super::BeginPlay();

	// ...
	
}


// Called every frame
void UDurationEffectComponent::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);

	// ...
}

UAbilitySystemComponent* UDurationEffectComponent::GetAsc()
{
	if (IsValid(OwnerAsc))
	{
		return  OwnerAsc;
	}
	else
	{
		this->OwnerAsc=Cast<UAbilitySystemComponent>(this->GetOwner()->GetComponentByClass(UAbilitySystemComponent::StaticClass()));
		return OwnerAsc;
	}
}

bool UDurationEffectComponent::IsEffectRemaining(const FGameplayTag& EffectTag)
{
	if (!GetAsc()) return false;
	FGameplayEffectQuery Query;
	Query.EffectTagQuery = FGameplayTagQuery::MakeQuery_MatchAnyTags(FGameplayTagContainer(EffectTag));
	auto ActiveEffectHandle=OwnerAsc->GetActiveEffects(Query);
	if (ActiveEffectHandle.Num()>0)
	{
		auto ActiveEffect= OwnerAsc->GetActiveGameplayEffect(ActiveEffectHandle[0]);
		int StackCount=ActiveEffect->Spec.GetStackCount();
		bool IsUsing=StackCount>1;
		return IsUsing;
	}
	return false;
}

void UDurationEffectComponent::SetEffect(const FGameplayTag& EffectTag,UActorComponent* InEffectComponent)
{
	if (DurationEffects.Find(EffectTag.GetTagName())==nullptr)
	{
		DurationEffects.Add(EffectTag.GetTagName(),InEffectComponent);
	}
}

bool UDurationEffectComponent::RemoveEffect(const FGameplayTag& EffectTag)
{
	auto EffectComponent=DurationEffects.FindRef(EffectTag.GetTagName());
	if (EffectComponent)
	{
		EffectComponent->Deactivate();
		EffectComponent->DestroyComponent(true);
		DurationEffects.Remove(EffectTag.GetTagName());
		return  true;
	}
	return  false;
}
