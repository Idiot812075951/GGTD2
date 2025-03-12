// Fill out your copyright notice in the Description page of Project Settings.


#include "FireDamageExecution.h"
#include "GGTD2/DataAsset/GGTD2_FireDataAsset.h"
#include "AbilitySystemComponent.h"
#include "GGTD2_AttributeSet.h"
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

    const UGGTD2_AttributeSet* Attribute=Cast<UGGTD2_AttributeSet>(TargetASC->GetAttributeSet(UGGTD2_AttributeSet::StaticClass()));
    
    // 应用直接伤害
    OutExecutionOutput.AddOutputModifier(FGameplayModifierEvaluatedData(
        //HealthDef.AttributeToCapture,
        UGGTD2_AttributeSet::GetHealthAttribute(),
        EGameplayModOp::Additive,
        -FinalDamage
    ));

    // 触发爆炸逻辑
    if (FMath::FRand() <= FireData->ExplosionChance)
    {
        // 计算爆炸伤害
        float ExplosionDamage = FinalDamage * FireData->ExplosionDamageRatio;

        // 执行范围伤害
        TArray<AActor*> IgnoredActors;
        IgnoredActors.Add(TargetActor); // 避免重复伤害主目标

        UGameplayStatics::ApplyRadialDamageWithFalloff(
            TargetActor->GetWorld(),
            ExplosionDamage,
            ExplosionDamage * 0.5f,
            TargetActor->GetActorLocation(),
            FireData->ExplosionRadius * 0.5f,
            FireData->ExplosionRadius,
            1.0f,
            UDamageType::StaticClass(),
            IgnoredActors,
            SourceActor,
            SourceASC->GetAvatarActor()->GetInstigatorController()
        );

        // 对范围内其他目标施加燃烧效果
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
                //if (Hit.GetActor() == TargetActor) continue;
        
                if (UAbilitySystemComponent* HitASC = Cast<UAbilitySystemComponent>(Hit.GetActor()->GetComponentByClass(UAbilitySystemComponent::StaticClass())))
                {
                    FGameplayEffectSpecHandle SpecHandle = SourceASC->MakeOutgoingSpec(
                        FireData->BurnEffectClass, // 配置燃烧持续伤害GE
                        1.0f,
                        Context
                    );
                    HitASC->ApplyGameplayEffectSpecToSelf(*SpecHandle.Data.Get());
                }
            }
        }
    }
}