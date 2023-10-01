// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Character/FPSCharacterBase.h"
#include "Character/CharacterComponent/StateInterface.h"
#include "Character/CharacterComponent/HealthComponent.h"
#include "Camera/CameraComponent.h"
#include "InputActionValue.h"
#include "PlayerFPSCharacter.generated.h"

/**
 * 
 */
UCLASS()
class MULTIPLAYERFPS_API APlayerFPSCharacter : public AFPSCharacterBase,public IStateInterface
{
	GENERATED_BODY()
	
protected:
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
		class UInputMappingContext* IMC_PlayerInput;

	UPROPERTY(VisibleAnyWhere, BlueprintReadOnly, Category = "Camera")
		class UCameraComponent* FollowCamera;

	UPROPERTY(EditDefaultsOnly, Category = "HealthComponent")
		UHealthComponent* HealthComponent;

	virtual void Move(const FInputActionValue& Value);
	virtual void Look(const FInputActionValue& Value);

	virtual void OnDeath_Implementation()override;
	virtual void OnTakeDamage_Implementation() override;
	virtual void OnTakeArmorLosing_Implementation()override;

	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

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
