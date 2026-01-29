// Fill out your copyright notice in the Description page of Project Settings.


#include "GA_DragonRage.h"

#include "TargetActor_Radius_DragonRage.h"
#include "Abilities/Tasks/AbilityTask_PlayMontageAndWait.h"
#include "Abilities/Tasks/AbilityTask_WaitGameplayEvent.h"
#include "Abilities/Tasks/AbilityTask_WaitTargetData.h"
#include "Components/CapsuleComponent.h"
#include "Components/SphereComponent.h"
#include "GameFramework/PawnMovementComponent.h"
#include "GGTD2/GamePlay/Character/GGTD2_CharacterBase.h"
#include "GGTD2/Helper/GAS/GGTD2GASBlueprintLibrary.h"
#include "Kismet/GameplayStatics.h"
#include "Kismet/KismetMathLibrary.h"
#include "PhysicsEngine/PhysicsConstraintActor.h"
#include "PhysicsEngine/PhysicsConstraintComponent.h"

void UGA_DragonRage::ActivateAbility(const FGameplayAbilitySpecHandle Handle,
                                     const FGameplayAbilityActorInfo* ActorInfo, const FGameplayAbilityActivationInfo ActivationInfo,
                                     const FGameplayEventData* TriggerEventData)
{
	Super::ActivateAbility(Handle, ActorInfo, ActivationInfo, TriggerEventData);
	OwnerCharacter=Cast<AGGTD2_CharacterBase>(ActorInfo->OwnerActor);
	//先选取敌人
	SpawnTargetActor();

}

void UGA_DragonRage::OnMontageNotifyBegin(FGameplayEventData Payload)
{
	FVector Impulse=(OwnerCharacter->GetActorForwardVector())*ImpulseScale+ImpulseOffset;
	if (UCapsuleComponent* TargetCapsule=TargetCharacter->GetCapsuleComponent())
	{
		TargetCapsule->SetSimulatePhysics(true);
		TargetCapsule->AddImpulse(Impulse,FName(""),true);
		UKismetSystemLibrary::PrintString(this,TEXT("UGA_DragonRage::OnMontageNotifyBegin"));
		UGGTD2GASBlueprintLibrary::Instakill(OwnerCharacter,TargetCharacter);
	}
	if (NotifyTask)
	{
		NotifyTask->EventReceived.RemoveDynamic(this, &UGA_DragonRage::OnMontageNotifyBegin);
	}
}

void UGA_DragonRage::OnMontageCancelled()
{
}

void UGA_DragonRage::SpawnTargetActor()
{
	FActorSpawnParameters SpawnParameters;
	SpawnParameters.bNoFail=true;
	SpawnParameters.Owner=GetCurrentActorInfo()->OwnerActor.Get();
	ATargetActor_Radius_DragonRage* TargetActor =GetWorld()->SpawnActor<ATargetActor_Radius_DragonRage>(ATargetActor_Radius_DragonRage::StaticClass(),GetCurrentActorInfo()->OwnerActor->GetActorTransform(),SpawnParameters);
	TargetActor->SetRadius(100.f);
	TargetActor->SetTargetSelectMode(EGATargetSelection::Closest);
	UAbilityTask_WaitTargetData* TargetTask = UAbilityTask_WaitTargetData::WaitTargetDataUsingActor(
		this,
		NAME_None,
		EGameplayTargetingConfirmation::Instant, // 改为手动确认
		 TargetActor);
	TargetTask->ValidData.AddDynamic(this, &UGA_DragonRage::OnTargetSelected);
	TargetTask->Activate();
}

void UGA_DragonRage::OnTargetSelected(const FGameplayAbilityTargetDataHandle& TargetData)
{
	// 先获取基类指针再尝试转换
	if (const FGameplayAbilityTargetData* BaseData = TargetData.Get(0))
	{
		if (const FGameplayAbilityTargetData_ActorArray* ActorData = static_cast<const FGameplayAbilityTargetData_ActorArray*>(BaseData))
		{
			if (ActorData->TargetActorArray.Num() > 0)
			{
				if (AGGTD2_CharacterBase* Target = Cast<AGGTD2_CharacterBase>(ActorData->TargetActorArray[0]))
				{
					auto Character=this->GetCurrentActorInfo()->OwnerActor;
					auto Roatation=UKismetMathLibrary::FindLookAtRotation(Character->GetActorLocation(),Target->GetActorLocation());
					Character->SetActorRotation(Roatation);
					TargetCharacter=Target;
					GEngine->AddOnScreenDebugMessage(213,5,FColor::Red,"OnTargetSelected: "+TargetCharacter->GetName());
					DoWork();
					return;
				}
			}
		}
	}
	GEngine->AddOnScreenDebugMessage(123,10,FColor::Blue,"No Selectd Target");
	CommitAbilityCooldown(CurrentSpecHandle,CurrentActorInfo,CurrentActivationInfo,true);
	EndAbility(CurrentSpecHandle,CurrentActorInfo,CurrentActivationInfo,true,true);
}

void UGA_DragonRage::DoWork()
{
	// 1. 先播放施法蒙太奇
	UAbilityTask_PlayMontageAndWait* MontageTask = UAbilityTask_PlayMontageAndWait::CreatePlayMontageAndWaitProxy(
		this,
		NAME_None,
		Montage_Cast,
		1.0f,
		NAME_None,
		false); // 注意关闭自动结束技能
	MontageTask->OnCancelled.AddDynamic(this, &UGA_DragonRage::OnMontageCancelled);
	MontageTask->Activate();
	// 注意,施加GE只能在服务器上,这里要判断下
	if (this->HasAuthority(&this->CurrentActivationInfo))
	{
		UKismetSystemLibrary::PrintString(this,TEXT("UGA_DragonRage::DoWork"));
		// 2. 监听动画通知
		NotifyTask = UAbilityTask_WaitGameplayEvent::WaitGameplayEvent(
			this,
			AniNotifyTag,
			nullptr,
			false,
			false
		);
		NotifyTask->EventReceived.AddDynamic(this, &UGA_DragonRage::OnMontageNotifyBegin);
		NotifyTask->Activate();
	}
}

