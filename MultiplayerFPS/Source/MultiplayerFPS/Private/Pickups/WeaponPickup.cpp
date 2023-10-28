// Fill out your copyright notice in the Description page of Project Settings.

#include "Pickups/WeaponPickup.h"
#include "Character/FPSCharacterBase.h"
#include "Net/UnrealNetwork.h"

void AWeaponPickup::OnPickedUp(AFPSCharacterBase* Character)
{
	Super::OnPickedUp(Character);

	UWeaponComponent* CharacterWeaponComp = Character->GetWeaponComp();
	if (CharacterWeaponComp != nullptr)
	{
		CharacterWeaponComp->AddWeapon(WeaponType);
		Destroy();
	}
}
