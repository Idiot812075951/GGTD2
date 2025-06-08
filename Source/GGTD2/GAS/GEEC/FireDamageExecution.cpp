// Fill out your copyright notice in the Description page of Project Settings.


#include "FireDamageExecution.h"

#include <string>

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
    //下面这三个东西，从火球测试和正式火球，拿到的数据不一样，空了研究下
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
        FCollisionQueryParams Params = FCollisionQueryParams::DefaultQueryParam;
        Params.bDebugQuery=true;
        Params.AddIgnoredActor(SourceActor);
        FCollisionShape Sphere = FCollisionShape::MakeSphere(FireData->ExplosionRadius);
        if (TargetActor->GetWorld()->SweepMultiByChannel(
            OutHits,
            TargetActor->GetActorLocation(),
            TargetActor->GetActorLocation(),
            FQuat::Identity,
            ECC_GameTraceChannel1,
            Sphere,
            Params))
        {
            GEngine->AddOnScreenDebugMessage(123,5,FColor::Cyan,TargetActor->GetActorLocation().ToString());
            TArray<FGuid> Guids;
            for (auto& Hit : OutHits)
            {
                if (UAbilitySystemComponent* HitASC = Cast<UAbilitySystemComponent>(Hit.GetActor()->GetComponentByClass(UAbilitySystemComponent::StaticClass())))
                {
                    FGuid CurrentId =Hit.GetActor()->GetActorGuid();
                    if (Guids.Contains(CurrentId))
                    {
                        //草了，他这个检测范围有有时候会重复算，给他来个去重小操作，以后范围选取都要记得，考虑重复算的问题
                        continue;
                    }
                    Guids.Add(CurrentId);
                    FGameplayEffectSpecHandle IgniteSpecHandle = SourceASC->MakeOutgoingSpec(
                        FireData->GE_Explosion,
                        1.0f,
                        Context
                    );
                    
                    if (IgniteSpecHandle.IsValid())
                    {
                        // 获取目标最大生命值
                        if (const UGGTD2_AttributeSet* HitAttributeSet = Cast<UGGTD2_AttributeSet>(HitASC->GetAttributeSet(UGGTD2_AttributeSet::StaticClass())))
                        {
                            //这个SetSetByCallerMagnitude是不是在修改Attribute属性的？我想把上面的伤害值扩散出去
                            IgniteSpecHandle.Data->SetSetByCallerMagnitude(FGameplayTag::RequestGameplayTag(FName("GameplayAbility.GE.SetByCaller.Health")), -ExplosionDamage);
                            HitASC->ApplyGameplayEffectSpecToSelf(*IgniteSpecHandle.Data.Get());
                            RecordDamage+=ExplosionDamage;
                            //可以用下面的东西来检查SetByCaller是否生效
                            // const float Value = IgniteSpecHandle.Data->GetSetByCallerMagnitude(FGameplayTag::RequestGameplayTag(FName("Gameplay.GE.SetByCaller.Health")),false);
                            // UE_LOG(LogTemp, Warning, TEXT("SetByCaller Value: %f"), Value);
                            
                        }
                    }

                    if (!FireData->CanBurning)
                    {
                        return;;
                    }
                    //添加引燃
                    // AGGTD2_CharacterBase* HitCharacter = Cast<AGGTD2_CharacterBase>(Hit.GetActor());
                    // if (HitCharacter && AbilityClass)
                    //上面两行非必要的,但还是那句话，通过GA施加的ApplyGameplayEffectSpecToTarget和GE的TargetAsc->ApplyGameplayEffectSpecToSelf(*GEHandle.Data.Get());
                    // 拿到的数据有区别
                    {
                        if (const UGGTD2_AttributeSet* HitAttributeSet = Cast<UGGTD2_AttributeSet>(HitASC->GetAttributeSet(UGGTD2_AttributeSet::StaticClass())))
                        {
                            const float MaxHealth = HitAttributeSet->GetMaxHealth();
                            const float BurnDamage = MaxHealth * FireData->BurningPercent;
                            const float BurnTotalDamage = BurnDamage * FireData->BurningSeconds;
                            RecordDamage+=BurnTotalDamage;
                            //施加点燃的GE
                            const FGameplayEffectSpecHandle BurningHandle = SourceASC->MakeOutgoingSpec( FireData->GE_Burning,1.0f,Context);
                            BurningHandle.Data->SetSetByCallerMagnitude(FGameplayTag::RequestGameplayTag(FName("GameplayAbility.GE.SetByCaller.BurningTime")), FireData->BurningSeconds);
                            BurningHandle.Data->SetSetByCallerMagnitude(FGameplayTag::RequestGameplayTag(FName("GameplayAbility.GE.SetByCaller.Health")), -BurnDamage);
                            HitASC->ApplyGameplayEffectSpecToSelf(*BurningHandle.Data.Get());
                            
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
