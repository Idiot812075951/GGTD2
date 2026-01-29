// Fill out your copyright notice in the Description page of Project Settings.


#include "GameplayTagAnimNotify.h"

#include "AbilitySystemComponent.h"
#include "Abilities/GameplayAbilityTypes.h"
#include "Kismet/KismetSystemLibrary.h"

void UGameplayTagAnimNotify::Notify(USkeletalMeshComponent* MeshComp, UAnimSequenceBase* Animation,
                                    const FAnimNotifyEventReference& EventReference)
{
	if (auto AbilitySystem = MeshComp->GetOwner()->FindComponentByClass<UAbilitySystemComponent>()) {
		FGameplayEventData Payload;
		AbilitySystem->HandleGameplayEvent(NotifyTag, &Payload);
	}
}
