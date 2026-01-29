// Fill out your copyright notice in the Description page of Project Settings.


#include "GGTD2_AttributeSet.h"

#include "GameplayEffectExtension.h"
#include "GameFramework/Character.h"
#include "GGTD2/GamePlay/Character/GGTD2_CharacterBase.h"
#include "Kismet/GameplayStatics.h"
#include "Net/UnrealNetwork.h"


UGGTD2_AttributeSet::UGGTD2_AttributeSet()
{
	//TODO 这个似乎没生效，还是走的蓝图的初始化属性GE
	InitMaxHealth(100.0f); 
	InitHealth(GetMaxHealth()); 
}

void UGGTD2_AttributeSet::PostAttributeChange(const FGameplayAttribute& Attribute, float OldValue, float NewValue)
{
	Super::PostAttributeChange(Attribute, OldValue, NewValue);
}

void UGGTD2_AttributeSet::PostGameplayEffectExecute(const FGameplayEffectModCallbackData& Data)
{
	Super::PostGameplayEffectExecute(Data);
	// ============== 步骤1：判断是否是Health属性被修改（仅处理HP变更） ==============
	if (Data.EvaluatedData.Attribute == GetHealthAttribute())
	{
		// ============== 步骤2：获取目标ASC、Actor，做空指针安全判断 ==============
		UAbilitySystemComponent* TargetASC = &Data.Target;
		AActor* TargetActor = TargetASC ? TargetASC->GetOwnerActor() : nullptr;
		AGGTD2_CharacterBase* TargetCharacter = Cast<AGGTD2_CharacterBase>(TargetActor);

		if (!TargetASC || !TargetActor || !TargetCharacter)
		{
			UE_LOG(LogTemp, Warning, TEXT("[AttributeSet] 目标Actor/角色无效，跳过死亡逻辑处理"));
			return;
		}

		// ============== 步骤3：获取当前HP，限制在0~MaxHealth之间（防止负数） ==============
		float CurrentHealth = FMath::Clamp(GetHealth(), 0.0f, GetMaxHealth());
		// 同步修正HP值（避免出现负数HP，保证数据一致性）
		SetHealth(CurrentHealth);

		// ============== 步骤4：权威端（服务器）判断HP≤0，设置2秒后销毁 ==============
		if (TargetActor->HasAuthority() && CurrentHealth <= 0.0f && !TargetCharacter->IsPendingKillPending())
		{
			// 避免重复设置生命周期（防止多次触发HP≤0导致重复调用）
			if (TargetCharacter->GetLifeSpan() <= 0.0f)
			{
				UE_LOG(LogTemp, Log, TEXT("[AttributeSet] 目标[%s]HP归0，2秒后自动销毁"), *TargetCharacter->GetName());

				// 核心：设置Actor生命周期为2秒，2秒后UE自动销毁该角色
				TargetCharacter->SetLifeSpan(1.0f);
			}
		}
	}
}


void UGGTD2_AttributeSet::GetLifetimeReplicatedProps(TArray<class FLifetimeProperty>& OutLifetimeProps) const
{
	Super::GetLifetimeReplicatedProps(OutLifetimeProps);
	DOREPLIFETIME_CONDITION_NOTIFY(UGGTD2_AttributeSet, Health, COND_None, REPNOTIFY_Always);
	DOREPLIFETIME_CONDITION_NOTIFY(UGGTD2_AttributeSet, MaxHealth, COND_None, REPNOTIFY_Always);
}

void UGGTD2_AttributeSet::OnRep_Health(const FGameplayAttributeData& OldHealth) const
{
	GAMEPLAYATTRIBUTE_REPNOTIFY(UGGTD2_AttributeSet, Health,OldHealth);
}
void UGGTD2_AttributeSet::OnRep_MaxHealth(const FGameplayAttributeData& OldMaxHealth) const
{
	GAMEPLAYATTRIBUTE_REPNOTIFY(UGGTD2_AttributeSet, MaxHealth,OldMaxHealth);
}



