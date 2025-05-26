// Fill out your copyright notice in the Description page of Project Settings.


#include "GA_Fireball.h"

#include "AbilitySystemComponent.h"
#include "Kismet/KismetSystemLibrary.h"

void UGA_Fireball::ActivateAbility(const FGameplayAbilitySpecHandle Handle, const FGameplayAbilityActorInfo* ActorInfo,
                                   const FGameplayAbilityActivationInfo ActivationInfo, const FGameplayEventData* TriggerEventData)
{
	this->NetExecutionPolicy=EGameplayAbilityNetExecutionPolicy::ServerInitiated;
	// 确保在服务器执行
	if (!HasAuthority(&ActivationInfo))
	{
		CancelAbility(Handle, ActorInfo, ActivationInfo, true);
		return;
	}

	// 验证GE有效性
	if (!GE_FireDamage.Get())
	{
		UE_LOG(LogTemp, Error, TEXT("GE_FireDamage未配置！"));
		CancelAbility(Handle, ActorInfo, ActivationInfo, true);
		return;
	}

	// 创建Spec并附加DataAsset
	FGameplayEffectSpecHandle SpecHandle = MakeOutgoingGameplayEffectSpec(GE_FireDamage);
	if (!SpecHandle.IsValid()) return;
	//这个不能删，给我们的火元素GE添加数据呢
	FGameplayEffectContextHandle Context = SpecHandle.Data->GetContext();
	Context.AddSourceObject(FireData);
	// 射线检测参数
	const FVector Start = ActorInfo->AvatarActor->GetActorLocation();
	const FVector End = Start + ActorInfo->AvatarActor->GetActorForwardVector() * 1000.0f;
	TArray<TEnumAsByte<EObjectTypeQuery>> ObjectTypes;
	ObjectTypes.Add(UEngineTypes::ConvertToObjectType(ECC_Pawn));
	ObjectTypes.Add(UEngineTypes::ConvertToObjectType(ECC_WorldDynamic));

	TArray<AActor*> IgnoreActors;
	IgnoreActors.Add(ActorInfo->OwnerActor.Get());
	FHitResult HitResult;

	if (const bool bHit = UKismetSystemLibrary::LineTraceSingleForObjects(
		GetWorld(), Start, End, ObjectTypes,
		false, IgnoreActors, EDrawDebugTrace::ForOneFrame,
		HitResult, true
	))
	{
		const AActor* HitActor = HitResult.GetActor();
		UE_LOG(LogTemp, Log, TEXT("命中Actor: %s"), *GetNameSafe(HitActor));

		// 获取目标ASC
		UAbilitySystemComponent* TargetASC = Cast<UAbilitySystemComponent>(HitActor->GetComponentByClass(UAbilitySystemComponent::StaticClass()));
		if (!TargetASC)
		{
			UE_LOG(LogTemp, Error, TEXT("目标无ASC！"));
			CancelAbility(Handle, ActorInfo, ActivationInfo, true);
			return;
		}
		FGameplayAbilityTargetData_SingleTargetHit* TargetData = new FGameplayAbilityTargetData_SingleTargetHit(HitResult);
		FGameplayAbilityTargetDataHandle TargetDataHandle(TargetData);
		// 应用GE到目标ASC
		ApplyGameplayEffectSpecToTarget(Handle, ActorInfo, ActivationInfo,SpecHandle,TargetDataHandle);
		
	}

	CommitAbility(Handle, ActorInfo, ActivationInfo);
}
