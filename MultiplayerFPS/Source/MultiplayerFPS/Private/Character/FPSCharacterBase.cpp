// Fill out your copyright notice in the Description page of Project Settings.


#include "Character/FPSCharacterBase.h"
#include "MultiplayerFPS\MultiplayerFPS.h"
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
	//only the authority can go from this point on
	if (!HasAuthority()) {
		return;
	}
	SetHealth(MaxHealth);
	SetArmor(MaxArmor);
	//initial the weapon array
	constexpr int32 WeaponCount = ENUM_TO_INT32(EWeaponType::MAX);
	Weapons.Init(nullptr, WeaponCount);
	//initial the ammo array
	constexpr int32 AmmoCount = ENUM_TO_INT32(EAmmoType::MAX);
	Ammo.Init(50, AmmoCount);
	//add all weapons
	for (int32 i = 0; i < WeaponCount; ++i) {
		AddWeapon(static_cast<EWeaponType>(i));
	}
	//equip machinegun to make sure there is always an equipped weapon
	EquipWeapon(EWeaponType::MachineGun, false);
}

void AFPSCharacterBase::ServerCycleWeapons_Implementation(int32 Direction)
{
}

void AFPSCharacterBase::ServerEquipWeapon_Implementation(EWeaponType WeaponType)
{
}

bool AFPSCharacterBase::EquipWeapon(EWeaponType WeaponType, bool bPlaySound)
{
	return false;
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

int32 AFPSCharacterBase::GetWeaponAmmo(const EAmmoType AmmoType) const
{
	return int32();
}

void AFPSCharacterBase::AddWeapon(EWeaponType WeaponType)
{
}

void AFPSCharacterBase::ApplyDamage(float Damage, AFPSCharacterBase* DamageCauser)
{
}

void AFPSCharacterBase::ClientPlaySound_Implementation(USoundBase* Sound)
{
}

void AFPSCharacterBase::MulticastPlayAnimMontage_Implementation(UAnimMontage* AnimMontage)
{
}

void AFPSCharacterBase::GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps)const {
	Super::GetLifetimeReplicatedProps(OutLifetimeProps);

	DOREPLIFETIME_CONDITION(AFPSCharacterBase, Health, COND_OwnerOnly);
	DOREPLIFETIME_CONDITION(AFPSCharacterBase, Armor, COND_OwnerOnly);
	DOREPLIFETIME_CONDITION(AFPSCharacterBase, Weapon, COND_OwnerOnly);
	DOREPLIFETIME_CONDITION(AFPSCharacterBase, Weapons, COND_OwnerOnly);
	DOREPLIFETIME_CONDITION(AFPSCharacterBase, Ammo, COND_OwnerOnly);

}

