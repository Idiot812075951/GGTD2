// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Character.h"
#include "AbilitySystemComponent.h"
#include "GGTD2_CharacterBase.generated.h"

// 1. 定义事件类型
USTRUCT()
struct FEnemyDeathEvent
{
	GENERATED_BODY()
	UPROPERTY()
	AActor* Killer;     // 击杀者
	UPROPERTY()
	AActor* DeadEnemy;  // 被击杀的怪物
};


UCLASS()
class GGTD2_API AGGTD2_CharacterBase : public ACharacter
{
	GENERATED_BODY()

public:
	UPROPERTY(EditAnywhere,BlueprintReadWrite)
	UAbilitySystemComponent* Asc;
	// Sets default values for this character's properties
	AGGTD2_CharacterBase();

	/** Returns TopDownCameraComponent subobject **/
	FORCEINLINE class UCameraComponent* GetTopDownCameraComponent() const { return TopDownCameraComponent; }
	/** Returns CameraBoom subobject **/
	FORCEINLINE class USpringArmComponent* GetCameraBoom() const { return CameraBoom; }
	
	/** Top down camera */
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = Camera, meta = (AllowPrivateAccess = "true"))
	class UCameraComponent* TopDownCameraComponent;

	/** Camera boom positioning the camera above the character */
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = Camera, meta = (AllowPrivateAccess = "true"))
	class USpringArmComponent* CameraBoom;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = UI, meta = (AllowPrivateAccess = "true"))
	class UWidgetComponent* HPBar;
	
	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = EffectComponent, meta = (AllowPrivateAccess = "true"))
	class UDurationEffectComponent* EffectComponent;

	void Test();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	// Called to bind functionality to input
	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;

	
	
};
