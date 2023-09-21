// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "SuperSideScrollerCharacter.h"
#include "SuperSideScroller_Player.generated.h"

/**
 * 
 */
UCLASS()
class SUPERSIDESCROLLER_API ASuperSideScroller_Player : public ASuperSideScrollerCharacter
{
	GENERATED_BODY()
	
public:
	//Constructor
	ASuperSideScroller_Player();

	void SpawnProjectile();

	UFUNCTION(BlueprintPure)
		int32 GetCurrentNumberOfCollectibles() { return NumberOfCollectibles; }

	void IncrementNumberOfCollectibles(int32 Value);
	void IncreaseMovementPowerup();

protected:
	UPROPERTY(EditAnyWhere, Category = Input)
		class UInputMappingContext* IC_Character;
	UPROPERTY(EditAnyWhere, Category = Input)
		class UInputAction* IA_Sprint;
	UPROPERTY(EditAnyWhere, Category = input)
		class UInputAction* IA_Throw;
	UPROPERTY(EditAnyWhere)
		class UAnimMontage* ThrowMontage;
	

	//Override base character class function to setup our player input component
	virtual void SetupPlayerInputComponent(class UInputComponent* InputComponent) override;

	//Sprinting
	void Sprint();
	//StopSprinting
	void StopSprinting();
	void ThrowProjectile();
	void EndPowerup();
private:
	//Bool to control if we are sprinting. Failsafe.
	bool bIsSprinting;

	UPROPERTY(EditAnyWhere)
		TSubclassOf<class APlayerProjectile> PlayerProjectile;

	UPROPERTY()
		int32 NumberOfCollectibles;

	UPROPERTY()
		FTimerHandle PowerupHandle;

	UPROPERTY()
		bool bHasPowerupActive;

};
