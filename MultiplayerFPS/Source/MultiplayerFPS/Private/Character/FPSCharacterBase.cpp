// Fill out your copyright notice in the Description page of Project Settings.


#include "Character/FPSCharacterBase.h"
#include "Weapon/WeaponBase.h" 
#include "MultiplayerFPS\MultiplayerFPS.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "Kismet/GameplayStatics.h"
#include "Net/UnrealNetwork.h"

// Sets default values
AFPSCharacterBase::AFPSCharacterBase()
{
	// Set this character to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = false;

	GetCharacterMovement()->MaxWalkSpeed = 800.f;
	GetCharacterMovement()->JumpZVelocity = 600.f;
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
	/*when Direction == 1 the search will go forward
	and do the next weapon functionality
	when Direction == -1 the search will go backwards
	and do the previous weapon functionality*/
	const int32 WeaponCount = Weapons.Num();

	//cycle through each index in the weapons array and try to equip it 
	const int32 StartWeaponIndex = GET_CIRCULAR_ARRAY_INDEX(WeaponIndex + Direction, WeaponCount);

	for (int32 i = StartWeaponIndex; i != WeaponIndex; i = GET_CIRCULAR_ARRAY_INDEX(i + Direction, WeaponCount)) {
		//abort the for loop if the equip was successful
		if (EquipWeapon((EWeaponType)i)) {
			break;
		}
	}
}

void AFPSCharacterBase::ServerEquipWeapon_Implementation(EWeaponType WeaponType)
{
	EquipWeapon(WeaponType);
}

bool AFPSCharacterBase::EquipWeapon(EWeaponType WeaponType, bool bPlaySound)
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

// Called to bind functionality to input
void AFPSCharacterBase::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);
}

void AFPSCharacterBase::ArmorAbsorbDamage(float& Damage)
{
	//calculate how much damage was absorbed and sbustract that from the armor
	const float AbsorbDamage = Damage * ArmorAbsorption;
	const float RemainingArmmor = Armor - AbsorbDamage;

	SetArmor(RemainingArmmor);

	//Recalculate the damage
	Damage = (Damage * (1 - ArmorAbsorption)) - FMath::Min(RemainingArmmor, 0.f);

}

void AFPSCharacterBase::AddWeapon(EWeaponType WeaponType)
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
	NewWeapon->AttachToComponent(GetMesh(), FAttachmentTransformRules::SnapToTargetNotIncludingScale, "GripPoint");
	
}

void AFPSCharacterBase::ApplyDamage(float Damage, AFPSCharacterBase* DamageCauser)
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

void AFPSCharacterBase::ClientPlaySound_Implementation(USoundBase* Sound)
{
	UGameplayStatics::PlaySound2D(GetWorld(), Sound);
}

void AFPSCharacterBase::MulticastPlayAnimMontage_Implementation(UAnimMontage* AnimMontage)
{
	PlayAnimMontage(AnimMontage);
}

void AFPSCharacterBase::GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps)const {
	Super::GetLifetimeReplicatedProps(OutLifetimeProps);

	DOREPLIFETIME_CONDITION(AFPSCharacterBase, Health, COND_OwnerOnly);
	DOREPLIFETIME_CONDITION(AFPSCharacterBase, Armor, COND_OwnerOnly);
	DOREPLIFETIME_CONDITION(AFPSCharacterBase, Weapon, COND_OwnerOnly);
	DOREPLIFETIME_CONDITION(AFPSCharacterBase, Weapons, COND_OwnerOnly);
	DOREPLIFETIME_CONDITION(AFPSCharacterBase, Ammo, COND_OwnerOnly);

}

