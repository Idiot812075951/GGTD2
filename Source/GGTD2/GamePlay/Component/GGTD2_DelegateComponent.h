// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "AttributeSet.h"
#include "Components/ActorComponent.h"
#include "GGTD2_DelegateComponent.generated.h"

//DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FOnAttributeChange, FGameplayAttributeData, AttributeData);

UCLASS( ClassGroup=(Custom), meta=(BlueprintSpawnableComponent) )
class GGTD2_API UGGTD2_DelegateComponent : public UActorComponent
{
	GENERATED_BODY()

public:	
	// Sets default values for this component's properties
	UGGTD2_DelegateComponent();

protected:
	// Called when the game starts
	virtual void BeginPlay() override;

public:
	//FOnAttributeChange OnAttributeChange;
	// Called every frame
	virtual void TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction) override;

		
};
