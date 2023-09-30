// Fill out your copyright notice in the Description page of Project Settings.


#include "Weapon/WeaponBase.h"
#include "Components/SkeletalMeshComponent.h"

// Sets default values
AWeaponBase::AWeaponBase()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = false;

	Mesh = CreateDefaultSubobject<USkeletalMeshComponent>(TEXT("Mesh"));
	RootComponent = Mesh;

	AmmoFrom = CreateDefaultSubobject<USceneComponent>(TEXT("AmmoFrom"));
	AmmoFrom->SetupAttachment(RootComponent);

	bReplicates = true;
}

void AWeaponBase::SetOwner(AActor* NewOwner)
{
	Super::SetOwner(NewOwner);

	Owner = NewOwner;
}

void AWeaponBase::StartFire()
{
	if (!bWantsFire || GetWorldTimerManager().GetTimerRemaining(FireTimer) > 0.f) {
		return;
	}
	
	
}

void AWeaponBase::FireHitScan(FVector FireLocation, FVector FireDirection)
{
}

// Called when the game starts or when spawned
void AWeaponBase::BeginPlay()
{
	Super::BeginPlay();
	
}

void AWeaponBase::ServerStartFire_Implementation()
{
}

void AWeaponBase::ServerStopFire_Implementation()
{
}

// Called every frame
void AWeaponBase::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

