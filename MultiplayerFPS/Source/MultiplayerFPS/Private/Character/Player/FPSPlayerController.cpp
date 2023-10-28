// Fill out your copyright notice in the Description page of Project Settings.


#include "Character/Player/FPSPlayerController.h"
#include "Character/FPSCharacterBase.h"
#include "UI/HUDWidget.h"
#include "UI/PlayerMenu.h"


void AFPSPlayerController::UpdateHealthPercent(float HealthPercnet)
{
	if (HUDWidget) {
		HUDWidget->UpdateHealthPercent(HealthPercnet);
	}
}

void AFPSPlayerController::UpdateArmorPercent(float ArmorPercent)
{
	if (HUDWidget) {
		HUDWidget->UpdateArmorPercent(ArmorPercent);
	}
}

void AFPSPlayerController::BeginPlay()
{
	Super::BeginPlay();
	if (IsLocalPlayerController()) {
		if (BP_HUDWidget) {
			HUDWidget = CreateWidget<UHUDWidget>(this, BP_HUDWidget);
			HUDWidget->UpdateArmorPercent(1.0f);
			HUDWidget->UpdateHealthPercent(1.0f);
			HUDWidget->AddToViewport(1);
		}
	}

	if (IsLocalController()) {
		if (PlayerMenuClass) {
			PlayerMenu = CreateWidget<UPlayerMenu>(this, PlayerMenuClass);
			PlayerMenu->AddToViewport(0);
		}
	}
}

void AFPSPlayerController::ToggleScoreboard(){
	if (PlayerMenu != nullptr) {
		PlayerMenu->ToggleScoreboard();
	}
}

void AFPSPlayerController::ClientNotifyKill_Implementation(const FString& Name) {
	if (PlayerMenu != nullptr) {
		PlayerMenu->NotifyKill(Name);
	}
}

void AFPSPlayerController::ClientShowScoreboard_Implementation() {
	if (PlayerMenu != nullptr) {
		PlayerMenu->SetScoreboardVisibility(true);
	}
}

