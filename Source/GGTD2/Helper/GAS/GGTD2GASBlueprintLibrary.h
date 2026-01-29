// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "AbilitySystemBlueprintLibrary.h"
#include "GGTD2GASBlueprintLibrary.generated.h"

class UGGTD2_CommonGASConfig;
/**
 * 
 */
UCLASS()
class GGTD2_API UGGTD2GASBlueprintLibrary : public UAbilitySystemBlueprintLibrary
{
	GENERATED_BODY()
	
public:
	UFUNCTION(BlueprintCallable, Category = "Instakill", 
	meta = (DisplayName = "Instakill", Keywords = "秒杀"))
	static void Instakill(AActor* Instigator, AActor* Target);
	
private:
	static UGGTD2_CommonGASConfig* GASCommonConfig;
	// 获取配置（自动加载默认配置，无配置则报错）
	static UGGTD2_CommonGASConfig* GetGASCommonConfig();
};
