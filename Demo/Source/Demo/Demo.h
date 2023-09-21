// Copyright Epic Games, Inc. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"

UENUM(BlueprintType)
enum class DemoAbilityID : uint8 {
	None UMETA(DisplayName = "None"),
	Confirm UMETA(DisplayName = "Confirm"),
	Cancel UMETA(DisplayName = "Cancel"),
	Ability1 UMETA(DispalyName = "Ability1"),
	Ability2 UMETA(DispalyName = "Ability2"),
	Ability3 UMETA(DispalyName = "Ability3"),
	Ability4 UMETA(DispalyName = "Ability4"),
	Ability5 UMETA(DispalyName = "Ability5")
};