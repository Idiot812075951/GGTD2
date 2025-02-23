// Fill out your copyright notice in the Description page of Project Settings.


#include "GGTD2_CharacterBase.h"


// Sets default values
AGGTD2_CharacterBase::AGGTD2_CharacterBase()
{
 	// Set this character to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;
	Asc=CreateDefaultSubobject<UAbilitySystemComponent>(TEXT("AbilitySystemComponent"));
}

// Called when the game starts or when spawned
void AGGTD2_CharacterBase::BeginPlay()
{
	Super::BeginPlay();
	
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

