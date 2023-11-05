// Fill out your copyright notice in the Description page of Project Settings.


#include "Character/Player/FPSPlayerController.h"
#include "Character/FPSCharacterBase.h"
#include "Character/CharacterComponent/HealthComponent.h"
#include "UI/HUDWidget.h"
#include "UI/PlayerMenu.h"

void AFPSPlayerController::UpdateStatePercent(float HealthPercent, float ArmorPercent)
{
	if (IsLocalController()) {

		if (HUDWidget) {
			HUDWidget->UpdateHealthPercent(HealthPercent);
			HUDWidget->UpdateArmorPercent(ArmorPercent);
		}
	}
}

void AFPSPlayerController::OnPossess(APawn* PossessedPawn)
{
	Super::OnPossess(PossessedPawn);

	AFPSCharacterBase* CharacterBase = Cast<AFPSCharacterBase>(PossessedPawn);
}

void AFPSPlayerController::BeginPlay()
{
	Super::BeginPlay();
	if (IsLocalPlayerController()) {
		if (BP_HUDWidget) {
			HUDWidget = CreateWidget<UHUDWidget>(this, BP_HUDWidget);
			HUDWidget->AddToViewport(0);
		}
	}
}

void AFPSPlayerController::ToggleScoreboard(){

	if (IsLocalController()) {
		if (PlayerMenuClass) {
			PlayerMenu = CreateWidget<UPlayerMenu>(this, PlayerMenuClass);
			PlayerMenu->ToggleScoreboard();
			PlayerMenu->AddToViewport(0);
		}
	}
}

void AFPSPlayerController::ShowdownScoreboard()
{
	PlayerMenu->RemoveFromParent();
	PlayerMenu->Destruct();
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

