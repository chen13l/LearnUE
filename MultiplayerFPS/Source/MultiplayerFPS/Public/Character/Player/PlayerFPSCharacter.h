// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Character/FPSCharacterBase.h"
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
	UPROPERTY(EditAnyWhere, BlueprintReadOnly, Category = "PlayerInput")
		class UInputAction* IA_Move;
	UPROPERTY(EditAnyWhere, BlueprintReadOnly, Category = "PlayerInput")
		class UInputAction* IA_Look;
	UPROPERTY(EditAnyWhere, BlueprintReadOnly, Category = "PlayerInput")
		class UInputAction* IA_Jump;
	UPROPERTY(EditAnyWhere, BlueprintReadOnly, Category = "PlayerInput")
		class UInputMappingContext* IMC_PlayerInput;

	UPROPERTY(VisibleAnyWhere, BlueprintReadOnly, Category = "Camera")
		class UCameraComponent* FollowCamera;


	virtual void Move(const FInputActionValue& Value);
	virtual void Look(const FInputActionValue& Value);


	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:
	APlayerFPSCharacter();

	// Called every frame
	virtual void Tick(float DeltaTime) override;

	// Called to bind functionality to input
	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;


};
