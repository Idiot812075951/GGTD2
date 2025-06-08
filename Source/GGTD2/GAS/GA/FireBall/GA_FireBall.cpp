// Fill out your copyright notice in the Description page of Project Settings.


#include "GA_FireBall.h"

#include "TargetActor_Radius_FireBall.h"
#include "Abilities/Tasks/AbilityTask_PlayMontageAndWait.h"
#include "Abilities/Tasks/AbilityTask_WaitTargetData.h"
#include "GGTD2/Helper/ProjectileActor.h"

void UGA_FireBall::ActivateAbility(const FGameplayAbilitySpecHandle Handle, const FGameplayAbilityActorInfo* ActorInfo,
                                const FGameplayAbilityActivationInfo ActivationInfo, const FGameplayEventData* TriggerEventData)
{
	Super::ActivateAbility(Handle, ActorInfo, ActivationInfo, TriggerEventData);


	// 1. 先播放施法蒙太奇
	UAbilityTask_PlayMontageAndWait* MontageTask = UAbilityTask_PlayMontageAndWait::CreatePlayMontageAndWaitProxy(
		this,
		NAME_None,
		Montage_Cast,
		1.0f,
		NAME_None,
		false); // 注意关闭自动结束技能

	// 正确绑定委托方式（需要UFUNCTION标记）
	MontageTask->OnCompleted.AddDynamic(this, &UGA_FireBall::OnMontageComplete);
	MontageTask->OnCancelled.AddDynamic(this, &UGA_FireBall::OnMontageCancelled);
	MontageTask->Activate();
}

void UGA_FireBall::OnMontageComplete()
{
	FActorSpawnParameters SpawnParameters;
	SpawnParameters.bNoFail=true;
	SpawnParameters.Owner=GetCurrentActorInfo()->OwnerActor.Get();
	ATargetActor_Radius_FireBall* TargetActor =GetWorld()->SpawnActor<ATargetActor_Radius_FireBall>(ATargetActor_Radius_FireBall::StaticClass(),GetCurrentActorInfo()->OwnerActor->GetActorTransform(),SpawnParameters);
	// 2. 蒙太奇播放完成后启动目标选择
	UAbilityTask_WaitTargetData* TargetTask = UAbilityTask_WaitTargetData::WaitTargetDataUsingActor(
		this,
		NAME_None,
		EGameplayTargetingConfirmation::Instant, // 改为手动确认
		 TargetActor);
	TargetTask->ValidData.AddDynamic(this, &UGA_FireBall::OnTargetSelected);
	TargetTask->Activate();
}

void UGA_FireBall::OnMontageCancelled()
{
}

void UGA_FireBall::OnTargetSelected(const FGameplayAbilityTargetDataHandle& TargetData)
{
	// 先获取基类指针再尝试转换
	if (const FGameplayAbilityTargetData* BaseData = TargetData.Get(0))
	{
		// // 安全转换到Location类型（更通用的处理方式）
		// if (const FGameplayAbilityTargetData_LocationInfo* LocationData = 
		// 	static_cast<const FGameplayAbilityTargetData_LocationInfo*>(BaseData))
		// {
		// 	SpawnFireballToLocation(LocationData->TargetLocation.GetTargetingTransform().GetLocation());
		// }
		// 或者处理ActorArray类型（需确认实际使用的数据类型）
		// else if (const FGameplayAbilityTargetData_ActorArray* ActorData = 
		// 	static_cast<const FGameplayAbilityTargetData_ActorArray*>(BaseData))
		if (const FGameplayAbilityTargetData_ActorArray* ActorData = static_cast<const FGameplayAbilityTargetData_ActorArray*>(BaseData))
		{
			if (ActorData->TargetActorArray.Num() > 0)
			{
				if (AGGTD2_CharacterBase* Target = Cast<AGGTD2_CharacterBase>(ActorData->TargetActorArray[0]))
				{
					SpawnFireballToTarget(Target);
				}
			}
		}
	}
	EndAbility(CurrentSpecHandle,CurrentActorInfo,CurrentActivationInfo,true,true);
}

// 火球投射物生成函数
void UGA_FireBall::SpawnFireballToTarget(AGGTD2_CharacterBase* Target)
{
	const FVector StartLoc = GetAvatarActorFromActorInfo()->GetActorLocation();
	const FVector EndLoc = Target->GetActorLocation();

	// 创建Spec并附加DataAsset
	const FGameplayEffectSpecHandle ElementSpecHandle = MakeOutgoingGameplayEffectSpec(GE_ElementDamage);
	if (!ElementSpecHandle.IsValid()) return;
	FGameplayEffectContextHandle Context = ElementSpecHandle.Data->GetContext();
	Context.AddSourceObject(FireData);
	Context.AddInstigator(GetCurrentActorInfo()->AvatarActor.Get(),GetCurrentActorInfo()->OwnerActor.Get());
	Context.SetAbility(this);
	ElementSpecHandle.Data->SetContext(Context);
	const FGameplayEffectSpecHandle SpecHandle = MakeOutgoingGameplayEffectSpec(GE_GADamage);
	FGameplayEffectContextHandle Context2 = SpecHandle.Data->GetContext();
	Context2.AddInstigator(GetCurrentActorInfo()->AvatarActor.Get(),GetCurrentActorInfo()->OwnerActor.Get());
	Context2.SetAbility(this);
	SpecHandle.Data->SetSetByCallerMagnitude(FGameplayTag::RequestGameplayTag(FName("GameplayAbility.GA.FireBall_Damage")), -FireBallDamage);
	SpecHandle.Data->SetContext(Context2);
	TArray<FGameplayEffectSpecHandle> GEHandles;
	GEHandles.Add(ElementSpecHandle);
	GEHandles.Add(SpecHandle);
    
	if (ProjectileActorClass)
	{
		AProjectileActor* ProjectileActor = GetWorld()->SpawnActor<AProjectileActor>(
			ProjectileActorClass,
			StartLoc,
			(EndLoc - StartLoc).Rotation());
        //下面这句应该是要获取它的投射物组件，然后再设置追踪目标
		ProjectileActor->FireTarget(Target,GEHandles);
	}
}