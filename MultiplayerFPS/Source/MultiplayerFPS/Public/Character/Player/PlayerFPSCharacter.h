// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Character/FPSCharacterBase.h"
#include "Character/CharacterComponent/HealthComponent.h"
#include "Camera/CameraComponent.h"
#include "InputActionValue.h"
#include "PlayerFPSCharacter.generated.h"

/**
 * 
 */
UCLASS()
class MULTIPLAYERFPS_API APlayerFPSCharacter : public AFPSCharacterBase
{
	GENERATED_BODY()
	
protected:
	UPROPERTY(EditDefaultsOnly,BlueprintReadOnly,Category="Player Comp")
		class ULookAtComponent* ViewComp;

	UPROPERTY(EditAnyWhere, BlueprintReadOnly, Category = "PlayerInput")
		class UInputAction* IA_Move;
	UPROPERTY(EditAnyWhere, BlueprintReadOnly, Category = "PlayerInput")
		class UInputAction* IA_Look;
	UPROPERTY(EditAnyWhere, BlueprintReadOnly, Category = "PlayerInput")
		class UInputAction* IA_Jump;

	UPROPERTY(EditAnyWhere, BlueprintReadOnly, Category = "PlayerInput")
		class UInputAction* IA_Fire;
	UPROPERTY(EditAnyWhere, BlueprintReadOnly, Category = "PlayerInput")
		class UInputAction* IA_MachineGun;
	UPROPERTY(EditAnyWhere, BlueprintReadOnly, Category = "PlayerInput")
		class UInputAction* IA_Pistol;
	UPROPERTY(EditAnyWhere, BlueprintReadOnly, Category = "PlayerInput")
		class UInputAction* IA_Railgun;
	UPROPERTY(EditAnyWhere, BlueprintReadOnly, Category = "PlayerInput")
		class UInputAction* IA_NextWeapon;
	UPROPERTY(EditAnyWhere, BlueprintReadOnly, Category = "PlayerInput")
		class UInputAction* IA_PreviousWeapon;
	UPROPERTY(EditAnyWhere, BlueprintReadOnly, Category = "PlayerInput")
		class UInputAction* IA_Scoreboard;
	UPROPERTY(EditAnyWhere, BlueprintReadOnly, Category = "PlayerInput")
		class UInputAction* IA_Pick;
	UPROPERTY(EditAnyWhere, BlueprintReadOnly, Category = "PlayerInput")
		class UInputMappingContext* IMC_PlayerInput;

	UPROPERTY(VisibleAnyWhere, BlueprintReadOnly, Category = "Camera")
		class UCameraComponent* FollowCamera;

	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

	//input
	virtual void Move(const FInputActionValue& Value);
	virtual void Look(const FInputActionValue& Value);

	void OnPressedFire();
	void OnReleaseFire();

	void OnPressedPistol();
	void OnPressedMachineGun();
	void OnPressedRailgun();
	void OnPressedPreviousWeapon();
	void OnPressedNextWeapon();

	void OnPressedPick();

	bool bHasOpenedScoreboard = false;
	void OnPressedScoreboard();
	void OnPressedShotdownScoreboard();
	
public:
	APlayerFPSCharacter();

	//camera
	FORCEINLINE FVector GetFollowCamera() const { return FollowCamera->GetComponentLocation(); }
	FORCEINLINE FVector GetCameraDirection()const { return GetControlRotation().Vector(); }

	// Called every frame
	virtual void Tick(float DeltaTime) override;

	// Called to bind functionality to input
	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;


};
