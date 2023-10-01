// Copyright Epic Games, Inc. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"

#define ENUM_TO_INT32(Value) (int32)Value
#define GET_WRAPPED_ARRAY_INDEX(Index, Count) (Index % Count + Count) % Count

UENUM(BlueprintType)
enum class EWeaponType :uint8 {
	Pistol UMETA(DisplayName = "Psitol"),
	MachineGun UMETA(DisplayName = "MachineGun"),
	Railgun UMETA(DisplayName = "Railgun"),
	MAX
};

UENUM(BlueprintType)
enum class EWeaponFireMode :uint8 {
	Single UMETA(DisplayName = "Single"),
	Automatic UMETA(DisplayName = "Automatic")
};

UENUM(BlueprintType)
enum class EAmmoType :uint8 {
	PistolBullets UMETA(DisplayName = "PistolBullets"),
	MachineGunBullets UMETA(DisplayName = "MachineGunBullets"),
	Slugs UMETA(DisplayName = "Slugs"),
	MAX
};