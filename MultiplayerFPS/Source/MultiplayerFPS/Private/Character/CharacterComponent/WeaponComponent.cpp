// Fill out your copyright notice in the Description page of Project Settings.


#include "Character/CharacterComponent/WeaponComponent.h"
#include "Character/FPSCharacterBase.h"
#include "Net/UnrealNetwork.h"

// Sets default values for this component's properties
UWeaponComponent::UWeaponComponent()
{
	// Set this component to be initialized when the game starts, and to be ticked every frame.  You can turn these features
	// off to improve performance if you don't need them.
	PrimaryComponentTick.bCanEverTick = true;

	//initial the weapon array
	constexpr int32 WeaponCount = ENUM_TO_INT32(EWeaponType::MAX);
	Weapons.Init(nullptr, WeaponCount);

	//initial the ammo array
	constexpr int32 AmmoCount = ENUM_TO_INT32(EAmmoType::MAX);
	Ammo.Init(0, AmmoCount);

}


// Called when the game starts
void UWeaponComponent::BeginPlay()
{
	Super::BeginPlay();

}

void UWeaponComponent::EndPlay(const EEndPlayReason::Type EndPlayReason)
{
	Super::EndPlay(EndPlayReason);

	// Destroy all of the weapons
	for (AWeaponBase* WeaponToDestroy : Weapons)
	{
		if (WeaponToDestroy != nullptr) {

			WeaponToDestroy->Destroy();
		}
	}
}

void UWeaponComponent::AddWeapon(EWeaponType WeaponType)
{
	//validate the add
	const int32 NewWeaponIndex = ENUM_TO_INT32(WeaponType);

	
	if (!WeaponClasses.IsValidIndex(NewWeaponIndex)) {
		return;
	}
	
	if (Weapons[NewWeaponIndex] != nullptr) {
		AddAmmo((EAmmoType)NewWeaponIndex, 20);
		return;
	}

	UClass* WeaponClass = WeaponClasses[NewWeaponIndex];

	if (WeaponClass == nullptr) {
		return;
	}

	//spawn the new waepon with this character as its owner
	FActorSpawnParameters SpawnParams = FActorSpawnParameters();
	SpawnParams.Owner = OwnerCharacter;
	SpawnParams.SpawnCollisionHandlingOverride = ESpawnActorCollisionHandlingMethod::AlwaysSpawn;

	AWeaponBase* NewWeapon = GetWorld()->SpawnActor<AWeaponBase>(WeaponClass, SpawnParams);

	if (NewWeapon == nullptr) {
		return;
	}

	//hide the weapon in the begining
	NewWeapon->SetActorHiddenInGame(true);

	//assign the new weapon to the respective index
	Weapons[NewWeaponIndex] = NewWeapon;
	AddAmmo((EAmmoType)NewWeaponIndex, 20);

	//attach the weapon to the right hand grip socket of the character
	NewWeapon->AttachToComponent(OwnerCharacter->GetMesh(), FAttachmentTransformRules::SnapToTargetNotIncludingScale, "GripPoint");

}


void UWeaponComponent::EquipWeapon(EWeaponType WeaponType, bool bPlaySound)
{
	//validate the equip
	const int32 NewWeaponIndex = ENUM_TO_INT32(WeaponType);

	if (!Weapons.IsValidIndex(NewWeaponIndex)) {
		return;
	}

	AWeaponBase* NewWeapon = Weapons[NewWeaponIndex];

	if (NewWeapon == nullptr || Weapon == NewWeapon || WeaponIndex == NewWeaponIndex) {
		bShouldWeaponChange = false;
		return;
	}

	//unequip the current weapon
	if (Weapon != nullptr) {
		Weapon->SetActorHiddenInGame(true);
	}

	//equip the new weapon
	Weapon = NewWeapon;
	WeaponIndex = NewWeaponIndex;

	Weapon->SetActorHiddenInGame(false);

	//play waepon change sound 
	if (WeaponChangedSound && bPlaySound) {
		OwnerCharacter->ClientPlaySound(WeaponChangedSound);
	}
	bShouldWeaponChange = true;
}

void UWeaponComponent::CycleWeapons(int32 Direction)
{
	/*when Direction == 1 the search will go forward and do the next weapon functionality
	when Direction == -1 the search will go backwards and do the previous weapon functionality*/
	const int32 WeaponCount = Weapons.Num();

	//cycle through each index in the weapons array and try to equip it 
	const int32 StartWeaponIndex = GET_CIRCULAR_ARRAY_INDEX(WeaponIndex + Direction, WeaponCount);

	for (int32 i = StartWeaponIndex; i != WeaponIndex; i = GET_CIRCULAR_ARRAY_INDEX(i + Direction, WeaponCount)) {
		//abort the for loop if the equip was successful
		if (!bShouldWeaponChange) {
			break;
		}
	}
}

void UWeaponComponent::GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps)const {
	Super::GetLifetimeReplicatedProps(OutLifetimeProps);

	DOREPLIFETIME_CONDITION(UWeaponComponent, Weapon, COND_OwnerOnly);
	DOREPLIFETIME_CONDITION(UWeaponComponent, Weapons, COND_OwnerOnly);
	DOREPLIFETIME_CONDITION(UWeaponComponent, Ammo, COND_OwnerOnly);
}


