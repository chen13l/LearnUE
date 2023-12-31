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
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "FPS Player Controller")
		TSubclassOf<class UHUDWidget> BP_HUDWidget;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "FPS Player Controller")
		TSubclassOf<class UPlayerMenu> PlayerMenuClass;

	virtual void BeginPlay()override;

private:
	UPROPERTY()
		class UHUDWidget* HUDWidget;

	UPROPERTY()
		class UPlayerMenu* PlayerMenu;

public:
	UFUNCTION()
		void UpdateStatePercent(float HealthPercent,float ArmorPercent);

	virtual void OnPossess(APawn* PossessedPawn) override;

	void ToggleScoreboard();
	void ShowdownScoreboard();

	UFUNCTION(Client, Reliable)
		void ClientNotifyKill(const FString& Name);

	UFUNCTION(Client, Reliable)
		void ClientShowScoreboard();
};
