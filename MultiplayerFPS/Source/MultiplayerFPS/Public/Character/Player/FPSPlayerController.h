// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/PlayerController.h"
#include "FPSPlayerController.generated.h"

/**
 * 
 */
UCLASS()
class MULTIPLAYERFPS_API AFPSPlayerController : public APlayerController
{
	GENERATED_BODY()
	
protected:
	UPROPERTY(EditDefaultsOnly)
		TSubclassOf<class UHUDWidget>BP_HUDWidget;

	virtual void BeginPlay()override;

private:
	UPROPERTY()
		class UHUDWidget* HUDWidget;

public:
	void UpdateHealthPercent(float HealthPercnet);
	void UpdateArmorPercent(float ArmorPercnet);
};
