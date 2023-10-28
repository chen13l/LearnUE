// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "MultiPlayerFPS/MultiplayerFPS.h"
#include "Pickups/PickupBase.h"
#include "WeaponPickup.generated.h"

UCLASS()
class MULTIPLAYERFPS_API AWeaponPickup : public APickupBase
{
	GENERATED_BODY()
	
protected:
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly,Category="Weapon Pickup")
		EWeaponType WeaponType;

	virtual void OnPickedUp(class AFPSCharacterBase* Character) override;
};
