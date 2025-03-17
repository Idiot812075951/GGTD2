// Fill out your copyright notice in the Description page of Project Settings.


#include "FireDamageExecution.h"
#include "GGTD2/DataAsset/GGTD2_FireDataAsset.h"
#include "AbilitySystemComponent.h"
#include "GGTD2/GGTD2_CharacterBase.h"
#include "GGTD2/GAS/GGTD2_AttributeSet.h"
#include "GGTD2/Helper/DamageStatisticsSystem.h"
#include "Kismet/GameplayStatics.h"


UFireDamageExecution::UFireDamageExecution()
{
    // 声明捕获的属性
    ArmorDef.AttributeToCapture = UGGTD2_AttributeSet::GetArmorAttribute();
    ArmorDef.AttributeSource = EGameplayEffectAttributeCaptureSource::Target;
    ArmorDef.bSnapshot = false;

    // HealthDef.AttributeToCapture = UGGTD2_AttributeSet::GetHealthAttribute();
    // HealthDef.AttributeSource = EGameplayEffectAttributeCaptureSource::Target;
    // HealthDef.bSnapshot = false;
    
    RelevantAttributesToCapture.Add(ArmorDef);
}

void UFireDamageExecution::Execute_Implementation(const FGameplayEffectCustomExecutionParameters& ExecutionParams, FGameplayEffectCustomExecutionOutput& OutExecutionOutput) const
{
    //重置记录伤害
    RecordDamage=0.f;
    
    // 获取Source和Target的ASC
    UAbilitySystemComponent* SourceASC = ExecutionParams.GetSourceAbilitySystemComponent();
    UAbilitySystemComponent* TargetASC = ExecutionParams.GetTargetAbilitySystemComponent();
    AActor* SourceActor = SourceASC ? SourceASC->GetAvatarActor() : nullptr;
    AActor* TargetActor = TargetASC ? TargetASC->GetAvatarActor() : nullptr;

    // 确保在服务器执行
    if (!SourceASC || !SourceASC->GetAvatarActor()->HasAuthority()) return;

    // 从Effect上下文中获取FireElementData
    FGameplayEffectContextHandle Context = ExecutionParams.GetOwningSpec().GetContext();
    UGGTD2_FireDataAsset* FireData = Cast<UGGTD2_FireDataAsset>(Context.GetSourceObject());
    if (FireData)
    {
        UE_LOG(LogTemp, Log, TEXT("FireData loaded: ExplosionChance = %f"), FireData->ExplosionChance);
    }
    else
    {
        UE_LOG(LogTemp, Error, TEXT("FireData could not be loaded"));
    }

    if (Context.GetSourceObject() && Context.GetSourceObject()->IsA<UGGTD2_FireDataAsset>())
    {
        FireData = Cast<UGGTD2_FireDataAsset>(Context.GetSourceObject());
    }

    if (!FireData) return;

    // 捕获护甲值
    float Armor = 0.0f;
    ExecutionParams.AttemptCalculateCapturedAttributeMagnitude(ArmorDef, FAggregatorEvaluateParameters(), Armor);

    // 计算基础伤害（考虑护甲减免）
    float FinalDamage = FireData->BaseDamage * (1 - FMath::Clamp(Armor / 100.0f, 0.0f, 1.0f));

    const UGGTD2_AttributeSet* Attribute = Cast<UGGTD2_AttributeSet>(TargetASC->GetAttributeSet(UGGTD2_AttributeSet::StaticClass()));

    // 应用直接伤害
    OutExecutionOutput.AddOutputModifier(FGameplayModifierEvaluatedData(
        UGGTD2_AttributeSet::GetHealthAttribute(),
        EGameplayModOp::Additive,
        -FinalDamage
    ));

    const FGameplayEffectSpec& Spec = ExecutionParams.GetOwningSpec();
    const UGameplayAbility* Ability = Spec.GetContext().GetAbilityInstance_NotReplicated();
    TSubclassOf<UGameplayAbility> AbilityClass = Ability ? Ability->GetClass() : nullptr;
    AGGTD2_CharacterBase* PlayerPawn = Cast<AGGTD2_CharacterBase>(SourceActor);
    // 记录伤害信息
    if (PlayerPawn && AbilityClass)
    {
        RecordAbilityClass=AbilityClass;
        RecordPlayerPawn=PlayerPawn;
    }

    // 触发爆炸逻辑
    if (FMath::FRand() <= FireData->ExplosionChance)
    {
        // 计算爆炸伤害
        float ExplosionDamage = FinalDamage * FireData->ExplosionDamageRatio;

        // 执行范围伤害（使用GAS框架）
        TArray<FHitResult> OutHits;
        FCollisionShape Sphere = FCollisionShape::MakeSphere(FireData->ExplosionRadius);
        if (TargetActor->GetWorld()->SweepMultiByChannel(
            OutHits,
            TargetActor->GetActorLocation(),
            TargetActor->GetActorLocation(),
            FQuat::Identity,
            ECC_GameTraceChannel1,
            Sphere))
        {
            for (auto& Hit : OutHits)
            {
                //先计算爆炸伤害
                if (UAbilitySystemComponent* HitASC = Cast<UAbilitySystemComponent>(Hit.GetActor()->GetComponentByClass(UAbilitySystemComponent::StaticClass())))
                {
                    FGameplayEffectSpecHandle IgniteSpecHandle = SourceASC->MakeOutgoingSpec(
                        UGameplayEffect::StaticClass(),
                        1.0f,
                        Context
                    );
                    
                    if (IgniteSpecHandle.IsValid())
                    {
                        if (IgniteSpecHandle.IsValid())
                        {
                            // 获取目标最大生命值
                            const UGGTD2_AttributeSet* HitAttributeSet = Cast<UGGTD2_AttributeSet>(HitASC->GetAttributeSet(UGGTD2_AttributeSet::StaticClass()));
                            if (HitAttributeSet)
                            {
                                FName HealthAttributeName = *UGGTD2_AttributeSet::GetHealthAttribute().GetName();
                                IgniteSpecHandle.Data->SetSetByCallerMagnitude(HealthAttributeName, -ExplosionDamage);
                                HitASC->ApplyGameplayEffectSpecToSelf(*IgniteSpecHandle.Data.Get());
                                RecordDamage+=ExplosionDamage;
                            }
                        }
                    }

                    if (!FireData->CanBurning)
                    {
                        return;;
                    }
                    //添加引燃
                    AGGTD2_CharacterBase* HitCharacter = Cast<AGGTD2_CharacterBase>(Hit.GetActor());
                    if (HitCharacter && AbilityClass)
                    {
                        if (const UGGTD2_AttributeSet* HitAttributeSet = Cast<UGGTD2_AttributeSet>(HitASC->GetAttributeSet(UGGTD2_AttributeSet::StaticClass())))
                        {
                            float MaxHealth = HitAttributeSet->GetMaxHealth();
                            float BurnDamage = MaxHealth * FireData->BurningPercent;
                            float BurnTotalDamage = BurnDamage * FireData->BurningSeconds;

                            // 复制一个新的UGameplayEffect实例
                            UGameplayEffect* NewBurnEffect = NewObject<UGameplayEffect>(GetTransientPackage(),UGameplayEffect::StaticClass());
                            NewBurnEffect->GameplayCues.Add( FGameplayEffectCue(FireData->BurningCueTag, 1, 1));
                            if (NewBurnEffect)
                            {
                                FGameplayEffectSpecHandle NewBurnSpecHandle = SourceASC->MakeOutgoingSpec(
                                     NewBurnEffect->GetClass(),
                                    1.0f,
                                    Context
                                );
                                //NewBurnSpecHandle.Data->SetDuration(FireData->BurningSeconds,false);
                                if (NewBurnSpecHandle.IsValid())
                                {
                                    HitASC->ApplyGameplayEffectSpecToSelf(*NewBurnSpecHandle.Data.Get());
                                }
                            }
                            RecordDamage+=BurnTotalDamage;
                        }
                    }
                }
            }
        }
    }

    // 广播伤害统计信息
    if (PlayerPawn && AbilityClass)
    {
        UDamageStatisticsSystem::GetInstance()->RecordDamage(RecordAbilityClass, RecordPlayerPawn, RecordDamage);
        UDamageStatisticsSystem::GetInstance()->BroadcastDamageStatistics(PlayerPawn, false);
    }

}
