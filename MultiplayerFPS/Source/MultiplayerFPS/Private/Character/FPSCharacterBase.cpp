// Fill out your copyright notice in the Description page of Project Settings.


#include "Character/FPSCharacterBase.h"
#include "Kismet/GameplayStatics.h"
#include "Net/UnrealNetwork.h"

// Sets default values
AFPSCharacterBase::AFPSCharacterBase()
{
 	// Set this character to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = false;
}

// Called when the game starts or when spawned
void AFPSCharacterBase::BeginPlay()
{
	Super::BeginPlay();
	
	if (SpawnSound) {
		UGameplayStatics::PlaySound2D(GetWorld(), SpawnSound);
	}
	SetHealth(MaxHealth);
	SetArmor(MaxArmor);
}

// Called every frame
void AFPSCharacterBase::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

// Called to bind functionality to input
void AFPSCharacterBase::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);
}

void AFPSCharacterBase::GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps)const {
	Super::GetLifetimeReplicatedProps(OutLifetimeProps);

	DOREPLIFETIME_CONDITION(AFPSCharacterBase, Health, COND_OwnerOnly);
	DOREPLIFETIME_CONDITION(AFPSCharacterBase, Armor, COND_OwnerOnly);
	DOREPLIFETIME_CONDITION(AFPSCharacterBase, Weapon, COND_OwnerOnly);
	DOREPLIFETIME_CONDITION(AFPSCharacterBase, Weapons, COND_OwnerOnly);
	DOREPLIFETIME_CONDITION(AFPSCharacterBase, Ammo, COND_OwnerOnly);

}
