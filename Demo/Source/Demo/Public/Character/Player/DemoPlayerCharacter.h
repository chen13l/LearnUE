// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Character/DemoCharacterBase.h"
#include "InputActionValue.h"
#include "DemoPlayerCharacter.generated.h"

/**
 * 
 */
UCLASS()
class DEMO_API ADemoPlayerCharacter : public ADemoCharacterBase
{
	GENERATED_BODY()
public:
	ADemoPlayerCharacter();

	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent)override;

	virtual void PossessedBy(AController* NewController)override;

	class USpringArmComponent* GetCameraBoom();

	class UCameraComponent* GetFollowCamera();

	UFUNCTION(BlueprintCallable, Category = "Demo|Camera")
		float GetStartingCameraBoomArmLength();

	UFUNCTION(BlueprintCallable, Category = "Demo|Camera")
		FVector GetStartingCameraBoomLocation();

protected:
	//for looking around and stuff 
	//because will remove the started controller and use this class instead to move the player 
	UPROPERTY(EditAnyWhere, BlueprintReadWrite, Category = "Demo|Camera")
		float BaseTurnRate = 45.f;
	UPROPERTY(EditAnyWhere, BlueprintReadWrite, Category = "Demo|Camera")
		float BaseLookUpRate = 45.f;

	//Camera
	UPROPERTY(BlueprintReadOnly, Category = "Demo|Camera")
		float StartingCameraBoomArmLength;
	UPROPERTY(BlueprintReadOnly, Category = "Demo|Camera")
		FVector StartingCameraBoomLocation;
	
	UPROPERTY(VisibleAnyWhere, BlueprintReadOnly, Category = "Demo|Camera")
		class UCameraComponent* FollowCamera;
	UPROPERTY(VisibleAnyWhere, BlueprintReadOnly, Category = "Demo|Camera")
		class USpringArmComponent* CameraBoom;

	//input 
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "PlayerInput")
		class UInputAction* IA_Move;
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "PlayerInput")
		class UInputAction* IA_Look;
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "PlayerInput")
		class UInputAction* IA_Jump;
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "PlayerInput")
		class UInputMappingContext* IMC_PlayerInput;

	//keep track of wheather bounded the ASC
	bool ASCInputBound = false;

	FGameplayTag DeadTag;

	virtual void BeginPlay()override;

	virtual void OnRep_PlayerState()override;

	//this is a helper function
	void InitializeStartingValues(class ADemoPlayerState* PS);

	void Look(const FInputActionValue& Value);
	void Move(const FInputActionValue& Value);

	void BindASCInput();
};
