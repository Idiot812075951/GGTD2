// Fill out your copyright notice in the Description page of Project Settings.


#include "TargetActor_Radius_FireBall.h"

#include "Kismet/GameplayStatics.h"

ATargetActor_Radius_FireBall::ATargetActor_Radius_FireBall()
{
}

void ATargetActor_Radius_FireBall::StartTargeting(UGameplayAbility* Ability)
{
	Super::StartTargeting(Ability);
}

void ATargetActor_Radius_FireBall::ConfirmTargetingAndContinue()
{
	//Super::ConfirmTargetingAndContinue();
	// 1. 执行球形检测获取范围内所有Actor
	this->Radius=800;
	const FVector Origin =this->OwningAbility.Get()->GetActorInfo().OwnerActor->GetActorLocation();
	TArray<TWeakObjectPtr<AActor>> OverlappedActors = PerformOverlap(Origin);

	// 2. 转换为强引用并过滤敌人
	TArray<AActor*> ValidEnemies;
	for (const auto& ActorPtr : OverlappedActors)
	{
		if (AActor* Actor = ActorPtr.Get())
		{
			if (Actor->IsA(AGGTD2_CharacterBase::StaticClass()) && 
				Actor->ActorHasTag("Enemy"))
			{
				ValidEnemies.Add(Actor);
			}
		}
	}

	// 3. 按距离排序并选择最远目标
	ValidEnemies.Sort([Origin](const AActor& A, const AActor& B) {
		return FVector::DistSquared(A.GetActorLocation(), Origin) > 
			   FVector::DistSquared(B.GetActorLocation(), Origin);
	});

	// 4. 构建目标数据
	FGameplayAbilityTargetDataHandle Handle;
	if (ValidEnemies.Num() > 0)
	{
		TArray<TWeakObjectPtr<AActor>> FinalTarget;
		FinalTarget.Add(ValidEnemies[0]);
		Handle = MakeTargetData(FinalTarget, Origin);
	}

	// 5. 回调通知
	TargetDataReadyDelegate.Broadcast(Handle);
}
