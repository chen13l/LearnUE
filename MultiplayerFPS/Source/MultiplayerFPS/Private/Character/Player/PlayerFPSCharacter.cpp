// Fill out your copyright notice in the Description page of Project Settings.


#include "Character/Player/PlayerFPSCharacter.h"
#include "Character\Player\FPSPlayerController.h"
#include "Camera/CameraComponent.h"
#include "Components/SkeletalMeshComponent.h"
#include "Components/InputComponent.h"
#include "Engine/World.h"
#include "EnhancedInputComponent.h"
#include "EnhancedInputSubsystems.h"
#include "..\..\..\Public\Character\Player\PlayerFPSCharacter.h"



APlayerFPSCharacter::APlayerFPSCharacter() {
	PrimaryActorTick.bCanEverTick = false;

	FollowCamera = CreateDefaultSubobject<UCameraComponent>(TEXT("FollowCamera"));
	FollowCamera->SetupAttachment(GetMesh(), "Camera");
	FollowCamera->bUsePawnControlRotation = true;
}


void  APlayerFPSCharacter::BeginPlay() {
	Super::BeginPlay();
	
}



void APlayerFPSCharacter::Tick(float DeltaTime) {
	Super::Tick(DeltaTime);

}

void APlayerFPSCharacter::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent) {
	Super::SetupPlayerInputComponent(PlayerInputComponent);

	UEnhancedInputComponent* EnhancedPlayerInput = Cast<UEnhancedInputComponent>(PlayerInputComponent);
	if (EnhancedPlayerInput) {
		APlayerController* PlayerController = Cast<APlayerController>(GetController());
		UEnhancedInputLocalPlayerSubsystem* InputSubsystem = ULocalPlayer::GetSubsystem<UEnhancedInputLocalPlayerSubsystem>(PlayerController->GetLocalPlayer());
		if (InputSubsystem) {
			InputSubsystem->AddMappingContext(IMC_PlayerInput, 0);
			//movement
			EnhancedPlayerInput->BindAction(IA_Move, ETriggerEvent::Triggered, this, &APlayerFPSCharacter::Move);
			EnhancedPlayerInput->BindAction(IA_Look, ETriggerEvent::Triggered, this, &APlayerFPSCharacter::Look);
			EnhancedPlayerInput->BindAction(IA_Jump, ETriggerEvent::Triggered, this, &ACharacter::Jump);
			EnhancedPlayerInput->BindAction(IA_Jump, ETriggerEvent::Completed, this, &ACharacter::StopJumping);
			//weapon
			EnhancedPlayerInput->BindAction(IA_Fire, ETriggerEvent::Triggered, this, &APlayerFPSCharacter::OnPressedFire);
			EnhancedPlayerInput->BindAction(IA_Fire, ETriggerEvent::Completed, this, &APlayerFPSCharacter::OnReleaseFire);
			EnhancedPlayerInput->BindAction(IA_Pistol, ETriggerEvent::Triggered, this, &APlayerFPSCharacter::OnPressedPistol);
			EnhancedPlayerInput->BindAction(IA_MachineGun, ETriggerEvent::Triggered, this, &APlayerFPSCharacter::OnPressedMachineGun);
			EnhancedPlayerInput->BindAction(IA_Railgun, ETriggerEvent::Triggered, this, &APlayerFPSCharacter::OnPressedRailgun);
			EnhancedPlayerInput->BindAction(IA_PreviousWeapon, ETriggerEvent::Triggered, this, &APlayerFPSCharacter::OnPressedPreviousWeapon);
			EnhancedPlayerInput->BindAction(IA_NextWeapon, ETriggerEvent::Triggered, this, &APlayerFPSCharacter::OnPressedNextWeapon);
			//other
			EnhancedPlayerInput->BindAction(IA_Scoreboard, ETriggerEvent::Triggered, this, &APlayerFPSCharacter::OnPressedScoreboard);
		}
	}
}


void APlayerFPSCharacter::Move(const FInputActionValue& Value)
{
	FVector MovementVector = Value.Get<FVector>();

	if (Controller) {
		FRotator Rotation = Controller->GetControlRotation();
		FRotator YawRotation = FRotator(0.f, Rotation.Yaw, 0.f);

		FVector ForwardDirection = FRotationMatrix(YawRotation).GetUnitAxis(EAxis::X);
		FVector RightDircetion = FRotationMatrix(YawRotation).GetUnitAxis(EAxis::Y);

		AddMovementInput(ForwardDirection, MovementVector.Y);
		AddMovementInput(RightDircetion, MovementVector.X);
	}
}

void APlayerFPSCharacter::Look(const FInputActionValue& Value)
{
	FVector LookAxis = Value.Get<FVector>();

	if (Controller) {
		AddControllerPitchInput(LookAxis.Y);
		AddControllerYawInput(LookAxis.X);
	}
}

void APlayerFPSCharacter::OnPressedFire()
{
	if (Weapon != nullptr) {
		Weapon->OnPressedFire();
	}
}

void APlayerFPSCharacter::OnReleaseFire()
{
	if (Weapon != nullptr) {
		Weapon->OnReleasedFire();
	}
}

void APlayerFPSCharacter::OnPressedPistol()
{
	ServerEquipWeapon(EWeaponType::Pistol);
}

void APlayerFPSCharacter::OnPressedMachineGun()
{
	ServerEquipWeapon(EWeaponType::MachineGun);
}

void APlayerFPSCharacter::OnPressedRailgun()
{
	ServerEquipWeapon(EWeaponType::Railgun);
}

void APlayerFPSCharacter::OnPressedPreviousWeapon()
{
	ServerCycleWeapons(-1);
}

void APlayerFPSCharacter::OnPressedNextWeapon()
{
	ServerCycleWeapons(1);
}

void APlayerFPSCharacter::OnPressedScoreboard()
{
	AFPSPlayerController* PlayerController = Cast<AFPSPlayerController>(GetController());

	if (PlayerController != nullptr) {
		PlayerController->ToggleScoreboard();
	}
}
