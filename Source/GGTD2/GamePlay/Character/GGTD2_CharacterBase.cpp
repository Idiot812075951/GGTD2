// Fill out your copyright notice in the Description page of Project Settings.


#include "GGTD2_CharacterBase.h"
#include "Components/CapsuleComponent.h"
#include "Components/WidgetComponent.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "GGTD2/Components/DurationEffectComponent.h"
#include "GGTD2/UI/AttributeSetWidget.h"


class UAttributeSetWidget;
// Sets default values
AGGTD2_CharacterBase::AGGTD2_CharacterBase()
{
 	// Set this character to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;
	Asc=CreateDefaultSubobject<UAbilitySystemComponent>(TEXT("AbilitySystemComponent"));
	EffectComponent=CreateDefaultSubobject<UDurationEffectComponent>(TEXT("EffectComponent"));

	// Set size for player capsule
	GetCapsuleComponent()->InitCapsuleSize(42.f, 96.0f);

	// Don't rotate character to camera direction
	bUseControllerRotationPitch = false;
	bUseControllerRotationYaw = false;
	bUseControllerRotationRoll = false;

	// Configure character movement
	GetCharacterMovement()->bOrientRotationToMovement = true; // Rotate character to moving direction
	GetCharacterMovement()->RotationRate = FRotator(0.f, 640.f, 0.f);
	GetCharacterMovement()->bConstrainToPlane = true;
	GetCharacterMovement()->bSnapToPlaneAtStart = true;

	HPBar = CreateDefaultSubobject<UWidgetComponent>(TEXT("HPBar"));
	HPBar->SetupAttachment(GetCapsuleComponent());

	// Activate ticking in order to update the cursor every frame.
	PrimaryActorTick.bCanEverTick = true;
	PrimaryActorTick.bStartWithTickEnabled = true;
	
}

void AGGTD2_CharacterBase::Test()
{
	UAttributeSetWidget* Widget=Cast<UAttributeSetWidget>(this->HPBar->GetWidget());
	if (Widget)
	{
		Widget->BindCharacter(this);
	}
}

// Called when the game starts or when spawned
void AGGTD2_CharacterBase::BeginPlay()
{
	Super::BeginPlay();

	if (IsNetMode(NM_Client))
	{
		
		FTimerHandle TimerHandle;
		GetWorldTimerManager().SetTimer(TimerHandle, this, &AGGTD2_CharacterBase::Test, 0.1,false);
		// 假设 UI 组件已附加到怪物

	}
}

// Called every frame
void AGGTD2_CharacterBase::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

// Called to bind functionality to input
void AGGTD2_CharacterBase::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);

}

