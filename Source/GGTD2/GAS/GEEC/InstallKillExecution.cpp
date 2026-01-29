// Fill out your copyright notice in the Description page of Project Settings.


#include "InstallKillExecution.h"

#include "GGTD2/GamePlay/Character/GGTD2_CharacterBase.h"
#include "GGTD2/GAS/GGTD2_AttributeSet.h"
#include "GGTD2/Helper/DamageStatisticsSystem.h"


void UInstallKillExecution::Execute_Implementation(const FGameplayEffectCustomExecutionParameters& ExecutionParams,
                                                   FGameplayEffectCustomExecutionOutput& OutExecutionOutput) const
{
	// 获取Source和Target的ASC
	UAbilitySystemComponent* SourceASC = ExecutionParams.GetSourceAbilitySystemComponent();
	UAbilitySystemComponent* TargetASC = ExecutionParams.GetTargetAbilitySystemComponent();
	AActor* SourceActor = SourceASC ? SourceASC->GetAvatarActor() : nullptr;
	AActor* TargetActor = TargetASC ? TargetASC->GetAvatarActor() : nullptr;
	
	const UGGTD2_AttributeSet* TargetAttributeSet = Cast<UGGTD2_AttributeSet>(
		   TargetASC->GetAttributeSet(UGGTD2_AttributeSet::StaticClass())
	   );
	if (!TargetAttributeSet)
	{
		UE_LOG(LogTemp, Warning, TEXT("[秒杀执行] 目标[%s]无有效AttributeSet，跳过秒杀"), *TargetActor->GetName());
		return;
	}

	// TODO 记录秒杀的伤害,这里需要1个接口,用来计算秒杀伤害
	float TargetCurrentHealth = TargetAttributeSet->GetHealth();
	RecordDamage += TargetCurrentHealth;

	// ============== 步骤4：符合GAS规范，将目标Health设为0（核心：通过OutExecutionOutput输出修改器） ==============
	FGameplayModifierEvaluatedData HealthZeroModifier;
	HealthZeroModifier.Attribute = UGGTD2_AttributeSet::GetHealthAttribute();
	HealthZeroModifier.ModifierOp = EGameplayModOp::Override;
	HealthZeroModifier.Magnitude = 0.f;

	// 将修改器添加到输出，让GAS生效（这一步是属性修改的关键，缺一不可）
	OutExecutionOutput.AddOutputModifier(HealthZeroModifier);
	AGGTD2_CharacterBase* PlayerPawn = Cast<AGGTD2_CharacterBase>(SourceActor);
	const FGameplayEffectSpec& Spec = ExecutionParams.GetOwningSpec();
	const UGameplayAbility* Ability = Spec.GetContext().GetAbilityInstance_NotReplicated();
	TSubclassOf<UGameplayAbility> AbilityClass = Ability ? Ability->GetClass() : nullptr;

	UDamageStatisticsSystem::GetInstance()->RecordDamage(AbilityClass, PlayerPawn, RecordDamage);
	UDamageStatisticsSystem::GetInstance()->BroadcastDamageStatistics(PlayerPawn, false);
	
	Super::Execute_Implementation(ExecutionParams, OutExecutionOutput);
	
}
