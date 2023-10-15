// Fill out your copyright notice in the Description page of Project Settings.


#include "Character/CharacterComponent/HealthComponent.h"
#include "Character\CharacterComponent\StateInterface.h"
#include "Character/FPSCharacterBase.h"
#include "Weapon/WeaponBase.h" 
#include "Net/UnrealNetwork.h"
#include "Engine/World.h"
#include "Kismet/GameplayStatics.h"


// Sets default values for this component's properties
UHealthComponent::UHealthComponent()
{
	// Set this component to be initialized when the game starts, and to be ticked every frame.  You can turn these features
	// off to improve performance if you don't need them.
	PrimaryComponentTick.bCanEverTick = false;

	SetHealth(MaxHealth);
	SetArmor(MaxArmor);

	//initial the ammo array
	constexpr int32 AmmoCount = ENUM_TO_INT32(EAmmoType::MAX);
	Ammo.Init(50, AmmoCount);
}

float UHealthComponent::ArmorAbsorbDamage(float& Damage)
{
	//calculate how much damage was absorbed and sbustract that from the armor
	const float AbsorbDamage = Damage * ArmorAbsorption;
	const float RemainingArmmor = FMath::Max(Armor - AbsorbDamage, 0.f);

	SetArmor(RemainingArmmor);

	//Recalculate the damage
	if (!RemainingArmmor) {
		float RemainingDamage = (Damage * (1 - ArmorAbsorption)) - FMath::Min(RemainingArmmor, 0.f);
		return RemainingDamage;
	}
	return 0;
}

void UHealthComponent::ApplyDamage(float Damage, AFPSCharacterBase* DamageCauser)
{
	if (IsDead()) {
		AFPSCharacterBase* Player = Cast<AFPSCharacterBase>(GetOwner());
		if (Player != nullptr) {
			Player->Destroy();
		}
	}
	float RemainingDamage = 0;
	//deduct the armor and the health
	if (Armor > 0) {
		RemainingDamage = ArmorAbsorbDamage(Damage);
		RemoveHealth(RemainingDamage);
	}
	else {
		RemoveHealth(Damage);
	}
	//play the hit sound on the owning client of the damage causer
	if (HitSound != nullptr && DamageCauser != nullptr) {
		DamageCauser->ClientPlaySound(HitSound);
	}
}

void UHealthComponent::GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps)const {
	Super::GetLifetimeReplicatedProps(OutLifetimeProps);

	DOREPLIFETIME_CONDITION(UHealthComponent, Health, COND_OwnerOnly);
	DOREPLIFETIME_CONDITION(UHealthComponent, Armor, COND_OwnerOnly);
	DOREPLIFETIME_CONDITION(UHealthComponent, Ammo, COND_OwnerOnly);
}

