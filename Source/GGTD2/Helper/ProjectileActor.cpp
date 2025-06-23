// Fill out your copyright notice in the Description page of Project Settings.


#include "ProjectileActor.h"

#include "GameplayEffectTypes.h"
#include "GameFramework/ProjectileMovementComponent.h"
#include "Components/SphereComponent.h"
#include "GGTD2/GamePlay/Character/GGTD2_CharacterBase.h"


// Sets default values
AProjectileActor::AProjectileActor()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;
	this->SetReplicates(true);
	this->StaticMeshComp=CreateDefaultSubobject<UStaticMeshComponent>(TEXT("UStaticMeshComponent"));
	this->SphereComponent=CreateDefaultSubobject<USphereComponent>(TEXT("SphereComponent"));
	this->ProjectileComp=CreateDefaultSubobject<UProjectileMovementComponent>(TEXT("UProjectileMovementComponent"));
}

// Called when the game starts or when spawned
void AProjectileActor::BeginPlay()
{
	Super::BeginPlay();
	this->OnActorBeginOverlap.AddDynamic(this,&ThisClass::OnOverLap);
}

void AProjectileActor::EndPlay(const EEndPlayReason::Type EndPlayReason)
{
	Super::EndPlay(EndPlayReason);
	this->OnActorBeginOverlap.RemoveDynamic(this,&ThisClass::OnOverLap);
}

void AProjectileActor::FireTarget(AActor* Target,TArray<FGameplayEffectSpecHandle> InGEHandles)
{
	if (AGGTD2_CharacterBase* CharacterBase=Cast<AGGTD2_CharacterBase>(Target))
	{
		this->TargetAsc=CharacterBase->Asc;
	}
	this->GEHandles=InGEHandles;
	this->ProjectileComp->bIsHomingProjectile=true;
	if (IsValid(Target))
	{
		this->ProjectileComp->HomingTargetComponent=Target->GetRootComponent();
	}
}

void AProjectileActor::OnOverLap(AActor* OverlappedActor, AActor* OtherActor)
{
	if (not IsValid(TargetAsc) or not HasAuthority())
	{
		return;
	}
	for (auto const GEHandle:this->GEHandles)
	{
		TargetAsc->ApplyGameplayEffectSpecToSelf(*GEHandle.Data.Get());
	}
	this->Destroy();
}

// Called every frame
void AProjectileActor::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

