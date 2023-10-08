// Fill out your copyright notice in the Description page of Project Settings.


#include "Character/CharacterComponent/HealthComponent.h"
#include "Character\CharacterComponent\StateInterface.h"
#include "Character/FPSCharacterBase.h"
#include "Weapon/WeaponBase.h" 
#include "Net/UnrealNetwork.h"


// Sets default values for this component's properties
UHealthComponent::UHealthComponent()
{
	// Set this component to be initialized when the game starts, and to be ticked every frame.  You can turn these features
	// off to improve performance if you don't need them.
	PrimaryComponentTick.bCanEverTick = false;

	// ...
}


// Called when the game starts
void UHealthComponent::BeginPlay()
{
	Super::BeginPlay();

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


// Called every frame
void UHealthComponent::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);

	// ...
}

//State
void UHealthComponent::LoseState(float Amount)
{
	const float AbsorbedDamage = Amount * ArmorAbsorption;
	const float RemainingArmor = Armor - AbsorbedDamage;
	Armor = RemainingArmor;
	if (GetOwner()->Implements<UStateInterface>()) {
		IStateInterface::Execute_OnTakeArmorLosing(GetOwner());
	}
	Amount = (Amount * (1 - ArmorAbsorption)) - FMath::Min(RemainingArmor, 0.0f);

	if (Armor <= 0.f) {
		Armor = 0.f;
		Health -= Amount;
		IStateInterface::Execute_OnTakeDamage(GetOwner());
		if (Health <= 0.f) {
			Health = 0.f;
			IStateInterface::Execute_OnDeath(GetOwner());
		}
	}
}


void UHealthComponent::ArmorAbsorbDamage(float& Damage)
{
	//calculate how much damage was absorbed and sbustract that from the armor
	const float AbsorbDamage = Damage * ArmorAbsorption;
	const float RemainingArmmor = Armor - AbsorbDamage;

	SetArmor(RemainingArmmor);

	//Recalculate the damage
	Damage = (Damage * (1 - ArmorAbsorption)) - FMath::Min(RemainingArmmor, 0.f);

}

void UHealthComponent::ApplyDamage(float Damage, AFPSCharacterBase* DamageCauser)
{
	if (IsDead()) {
		return;
	}

	//deduct the armor and the health
	ArmorAbsorbDamage(Damage);
	RemoveHealth(Damage);

	//play the hit sound on the owning client of the damage causer
	if (HitSound != nullptr && DamageCauser != nullptr) {
		DamageCauser->ClientPlaySound(HitSound);
	}
}

//Weapon
void UHealthComponent::AddWeapon(EWeaponType WeaponType)
{
	//validate the add
	const int32 NewWeaponIndex = ENUM_TO_INT32(WeaponType);

	if (!WeaponClasses.IsValidIndex(NewWeaponIndex) || Weapons[NewWeaponIndex] != nullptr) {
		return;
	}

	UClass* WeaponClass = WeaponClasses[NewWeaponIndex];

	if (WeaponClass == nullptr) {
		return;
	}

	//spawn the new waepon with this character as its owner
	FActorSpawnParameters SpawnParams = FActorSpawnParameters();
	SpawnParams.Owner = this;
	SpawnParams.SpawnCollisionHandlingOverride = ESpawnActorCollisionHandlingMethod::AlwaysSpawn;

	AWeaponBase* NewWeapon = GetWorld()->SpawnActor<AWeaponBase>(WeaponClass, SpawnParams);

	if (NewWeapon == nullptr) {
		return;
	}

	//hide the weapon in the begining
	NewWeapon->SetActorHiddenInGame(true);

	//assign the new weapon to the respective index
	Weapons[NewWeaponIndex] = NewWeapon;

	//attach the weapon to the right hand grip socket of the character
	NewWeapon->AttachToComponent(Cast<AFPSCharacterBase>(GetOwner())->GetMesh(), FAttachmentTransformRules::SnapToTargetNotIncludingScale, "GripPoint");

}

bool UHealthComponent::EquipWeapon(EWeaponType WeaponType, bool bPlaySound)
{
	//validate the equip
	const int32 NewWeaponIndex = ENUM_TO_INT32(WeaponType);

	if (!Weapons.IsValidIndex(NewWeaponIndex)) {
		return false;
	}

	AWeaponBase* NewWeapon = Weapons[NewWeaponIndex];

	if (NewWeapon == nullptr || Weapon == NewWeapon) {
		return false;
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
		ClientPlaySound(WeaponChangedSound);
	}

	return true;
}


void UHealthComponent::GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps)const {
	Super::GetLifetimeReplicatedProps(OutLifetimeProps);

	DOREPLIFETIME_CONDITION(UHealthComponent, Health, COND_OwnerOnly);
	DOREPLIFETIME_CONDITION(UHealthComponent, Armor, COND_OwnerOnly);
	DOREPLIFETIME_CONDITION(UHealthComponent, Weapon, COND_OwnerOnly);
	DOREPLIFETIME_CONDITION(UHealthComponent, Weapons, COND_OwnerOnly);
	DOREPLIFETIME_CONDITION(UHealthComponent, Ammo, COND_OwnerOnly);

}

