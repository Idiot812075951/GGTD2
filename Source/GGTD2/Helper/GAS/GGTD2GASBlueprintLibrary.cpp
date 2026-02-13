// Fill out your copyright notice in the Description page of Project Settings.


#include "GGTD2GASBlueprintLibrary.h"
#include "AbilitySystemComponent.h"
#include "GGTD2/DataAsset/GGTD2_CommonGASConfig.h"

UGGTD2_CommonGASConfig* UGGTD2GASBlueprintLibrary::GASCommonConfig =nullptr;

void UGGTD2GASBlueprintLibrary::Instakill(AActor* Instigator, AActor* Target,UGameplayAbility* GA)
{
	if (not Instigator->HasAuthority())
	{
		UE_LOG(LogTemp, Warning, TEXT(" 只能在服务器上施加GE"));
		return;
	}
	if (!Instigator || !Target)
	{
		UE_LOG(LogTemp, Warning, TEXT("[秒杀] 施法者/目标为空，跳过释放"));
		return;
	}
	UAbilitySystemComponent* TargetASC = GetAbilitySystemComponent(Target);
	if (!TargetASC->IsValidLowLevel())
	{
		UE_LOG(LogTemp, Warning, TEXT("[秒杀] 目标[%s]无有效ASC，跳过释放"), *Target->GetName());
		return;
	}
	
	FGameplayEffectContextHandle EffectContext = TargetASC->MakeEffectContext();
	EffectContext.SetAbility(GA);
	EffectContext.AddInstigator(Instigator,Instigator);
	EffectContext.AddSourceObject(Instigator);
	if (auto DefaultInstakillGE=GetGASCommonConfig()->GetEffectByName(FName("GE.InstallKill")))
	{
		FGameplayEffectSpecHandle EffectSpecHandle = TargetASC->MakeOutgoingSpec(DefaultInstakillGE, 1.0f, EffectContext);
		if (!EffectSpecHandle.IsValid()) return;
		TargetASC->ApplyGameplayEffectSpecToSelf(*EffectSpecHandle.Data.Get());
		UE_LOG(LogTemp, Warning, TEXT("[秒杀] 目标[%s]成功"), *Target->GetName());
	}
	else
	{
		UE_LOG(LogTemp, Warning, TEXT("DefaultInstakillGE is Null"));
	}
}

UGGTD2_CommonGASConfig* UGGTD2GASBlueprintLibrary::GetGASCommonConfig()
{
	// 1. 如果已设置配置，直接返回
	if (GASCommonConfig) return GASCommonConfig;

	// 2. 自动加载默认配置（替换成你项目的DataAsset路径）
	static const FString DefaultConfigPath = TEXT("/Script/GGTD2.GGTD2_CommonGASConfig'/Game/GGTD/Data/DA_GASCommonConfig.DA_GASCommonConfig'");
	GASCommonConfig = LoadObject<UGGTD2_CommonGASConfig>(nullptr, *DefaultConfigPath);

	// 3. 加载失败提示
	if (!GASCommonConfig)
	{
		UE_LOG(LogTemp, Error, TEXT("[秒杀] 默认配置加载失败！请检查路径：%s"), *DefaultConfigPath);
	}

	return GASCommonConfig;
}
