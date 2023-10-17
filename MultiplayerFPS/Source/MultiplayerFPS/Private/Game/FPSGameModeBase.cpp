// Fill out your copyright notice in the Description page of Project Settings.


#include "Game/FPSGameModeBase.h"
#include "Character\FPSCharacterBase.h"
#include "Character/Player/FPSPlayerController.h"
#include "Character\Player\FPSPlayerState.h"
#include "Game\FPSGameState.h"
#include "Engine/World.h"
#include "Engine/Public/TimerManager.h"

#include "Kismet/GameplayStatics.h"

AFPSGameModeBase::AFPSGameModeBase() {
	DefaultPawnClass = AFPSCharacterBase::StaticClass();
	PlayerControllerClass = AFPSPlayerController::StaticClass();
	PlayerStateClass = AFPSPlayerState::StaticClass();
	GameStateClass = AFPSGameState::StaticClass();
}

bool AFPSGameModeBase::ShouldSpawnAtStartSpot(AController* PlayerController) {
	return true;
}

void AFPSGameModeBase::HandleMatchHasStarted() {
	Super::HandleMatchHasStarted();

	AFPSGameState* FPSGameState = Cast<AFPSGameState>(GameState);

	if (FPSGameState != nullptr) {
		FPSGameState->SetKillLimit(KillLimit);
	}
}

void AFPSGameModeBase::HandleMatchHasEnded() {
	Super::HandleMatchHasEnded();

	TArray<AActor*> PlayerControllers;

	UGameplayStatics::GetAllActorsOfClass(this, AFPSPlayerController::StaticClass(), PlayerControllers);

	for (AActor* PlayerConroller : PlayerControllers) {
		AFPSPlayerController* FPSPlayerController = Cast<AFPSPlayerController>(PlayerConroller);

		if(FPSPlayerController == nullptr){
			continue;
		}

		APawn* Pawn = FPSPlayerController->GetPawn();

		if (Pawn != nullptr) {
			Pawn->Destroy();
		}

		FPSPlayerController->ClientShowScoreboard();
	}

	FTimerHandle TimerHandle;
	GetWorldTimerManager().SetTimer(TimerHandle, this, &AFPSGameModeBase::RestartMap, 5.0f);
}

bool AFPSGameModeBase::ReadyToEndMatch_Implementation()
{
	return HasWinner();
}

void AFPSGameModeBase::RestartMap() const
{
	GetWorld()->ServerTravel(GetWorld()->GetName(), false, false);
}

bool AFPSGameModeBase::HasWinner() const
{
	//go through the player states and check if any of the players have reached the kill limit

	for (APlayerState* PlayerState : GameState->PlayerArray) {
		AFPSPlayerState* FPSPlayerState = Cast<AFPSPlayerState>(PlayerState);

		if (FPSPlayerState != nullptr && FPSPlayerState->GetKills() == KillLimit) {
			return true;
		}
	}

	return false;
}

void AFPSGameModeBase::OnKill(AController* KillerController, AController* VictimController)
{
	if (!IsMatchInProgress())
	{
		return;
	}

	// Add kill to the killer

	if (KillerController != nullptr && KillerController != VictimController)
	{
		AFPSPlayerState* KillerPlayerState = Cast<AFPSPlayerState>(KillerController->PlayerState);

		if (KillerPlayerState != nullptr)
		{
			KillerPlayerState->AddKill();
		}

		// Show the kill on the killer's HUD

		AFPSPlayerController* KillerFPSController = Cast<AFPSPlayerController>(KillerController);

		if (KillerFPSController != nullptr && VictimController != nullptr && VictimController->PlayerState != nullptr)
		{
			KillerFPSController->ClientNotifyKill(VictimController->PlayerState->GetPlayerName());
		}
	}

	// Add death to the victim

	if (VictimController != nullptr)
	{
		AFPSPlayerState* VictimPlayerState = Cast<AFPSPlayerState>(VictimController->PlayerState);

		if (VictimPlayerState != nullptr)
		{
			VictimPlayerState->AddDeath();
		}

		APawn* Pawn = VictimController->GetPawn();

		if (Pawn != nullptr)
		{
			Pawn->Destroy();
		}

		if (!HasWinner())
		{
			RestartPlayer(VictimController);
			AFPSPlayerController* PlayerController = Cast<AFPSPlayerController>(VictimController);
			if (PlayerController) {
				PlayerController->UpdateHealthPercent(1.0f);
				PlayerController->UpdateArmorPercent(1.0f);
				UE_LOG(LogTemp, Error, TEXT("Has ReSpawned"));
			}
		}
	}

}
