// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "AbilitySystemComponent.h"
#include "GameFramework/Actor.h"
#include "ProjectileActor.generated.h"

class UProjectileMovementComponent;
class USphereComponent;
class ProjectileMovementComponent;

UCLASS()
class GGTD2_API AProjectileActor : public AActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	AProjectileActor();

	UPROPERTY(EditAnywhere,BlueprintReadWrite)
	USphereComponent* SphereComponent;

	UPROPERTY(EditAnywhere,BlueprintReadWrite)
	UStaticMeshComponent* StaticMeshComp;

	UPROPERTY(EditAnywhere,BlueprintReadWrite)
	UProjectileMovementComponent* ProjectileComp;

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;
	
	virtual void EndPlay(const EEndPlayReason::Type EndPlayReason) override;

public:
	void FireTarget(AActor* Target,TArray<FGameplayEffectSpecHandle> GEHandles);
	
	UFUNCTION()
	void OnOverLap(AActor* OverlappedActor, AActor* OtherActor);
	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

private:
	TArray<FGameplayEffectSpecHandle> GEHandles;

	UAbilitySystemComponent* TargetAsc;

};
