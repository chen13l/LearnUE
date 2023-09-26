// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "UObject/Interface.h"
#include "StateInterface.generated.h"

// This class does not need to be modified.
UINTERFACE(MinimalAPI)
class UStateInterface : public UInterface
{
	GENERATED_BODY()
};

/**
 * 
 */
class MULTIPLAYERFPS_API IStateInterface
{
	GENERATED_BODY()

	// Add interface functions to this class. This is the class that will be inherited to implement this interface.
public:
	UFUNCTION(BlueprintNativeEvent, Category = "States")
		void OnDeath();
	virtual void OnDeath_Implementation() = 0;

	UFUNCTION(BlueprintNativeEvent, Category = "States")
		void OnTakeDamage();
	virtual void OnTakeDamage_Implementation() = 0;

	UFUNCTION(BlueprintNativeEvent, Category = "States")
		void OnTakeArmorLosing();
	virtual void OnTakeArmorLosing_Implementation() = 0;
};
