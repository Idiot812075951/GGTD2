// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "AbilitySystemComponent.h"
#include "Components/ActorComponent.h"
#include "DurationEffectComponent.generated.h"


UCLASS( ClassGroup=(Custom), meta=(BlueprintSpawnableComponent) )
class GGTD2_API UDurationEffectComponent : public UActorComponent
{
	GENERATED_BODY()

public:	
	// Sets default values for this component's properties
	UDurationEffectComponent();

protected:
	// Called when the game starts
	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction) override;
	
	UFUNCTION(BlueprintCallable)
	UAbilitySystemComponent* GetAsc();

	UFUNCTION(BlueprintCallable)
	bool IsEffectRemaining(const FGameplayTag& EffectTag);
	UFUNCTION(BlueprintCallable)
	void SetEffect(const FGameplayTag& EffectTag,UActorComponent* InEffectComponent);
	
	UFUNCTION(BlueprintCallable)
	bool RemoveEffect(const FGameplayTag& EffectTag);
private:
	UPROPERTY()
	UAbilitySystemComponent* OwnerAsc;

	UPROPERTY()
	TMap<FName,UActorComponent*> DurationEffects;
};
