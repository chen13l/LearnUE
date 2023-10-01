// Fill out your copyright notice in the Description page of Project Settings.


#include "Character/Player/FPSPlayerController.h"
#include "UI/HUDWidget.h"

void AFPSPlayerController::UpdateHealthPercent(float HealthPercnet)
{
	if (HUDWidget) {
		HUDWidget->UpdateHealthPercent(HealthPercnet);
	}
}

void AFPSPlayerController::UpdateArmorPercent(float ArmorPercnet)
{
	if (HUDWidget) {
		HUDWidget->UpdateArmorPercent(ArmorPercnet);
	}
}

void AFPSPlayerController::BeginPlay()
{
	Super::BeginPlay();
	if (IsLocalPlayerController()) {
		if (BP_HUDWidget) {
			HUDWidget = CreateWidget<UHUDWidget>(this, BP_HUDWidget);
			HUDWidget->AddToViewport();
		}
	}
}
