// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/GameMode.h"
#include "FPSGameModeBase.generated.h"


/**
 * 
 */
UCLASS()
class MULTIPLAYERFPS_API AFPSGameModeBase : public AGameMode
{
	GENERATED_BODY()
	
protected:
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "FPS Game Mode")
		int32 KillLimit = 30;

	AFPSGameModeBase();

	virtual bool ShouldSpawnAtStartSpot(AController* PlayerController) override;
	virtual void HandleMatchHasStarted() override;
	virtual void HandleMatchHasEnded() override;
	virtual bool ReadyToEndMatch_Implementation() override;

	void RestartMap() const;

public:

	bool HasWinner() const;

	void OnKill(AController* KillerController, AController* VictimController);

};
