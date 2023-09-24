// Fill out your copyright notice in the Description page of Project Settings.


#include "Character/Player/PlayerFPSCharacter.h"
#include "Character\Player\FPSPlayerController.h"
#include "Character\CharacterComponent\HealthComponent.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "Camera/CameraComponent.h"
#include "Components/SkeletalMeshComponent.h"
#include "Components/InputComponent.h"
#include "Engine/World.h"
#include "EnhancedInputComponent.h"
#include "EnhancedInputSubsystems.h"



APlayerFPSCharacter::APlayerFPSCharacter() {
	PrimaryActorTick.bCanEverTick = false;

	GetCharacterMovement()->MaxWalkSpeed = 800.f;
	GetCharacterMovement()->JumpZVelocity = 600.f;

	FollowCamera = CreateDefaultSubobject<UCameraComponent>(TEXT("FollowCamera"));
	FollowCamera->SetupAttachment(GetMesh(), "Camera");
	FollowCamera->bUsePawnControlRotation = true;
}


void  APlayerFPSCharacter::BeginPlay() {
	Super::BeginPlay();

	UE_LOG(LogTemp, Warning, TEXT("The Health is %f"), GetHealth());
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
			EnhancedPlayerInput->BindAction(IA_Move, ETriggerEvent::Triggered, this, &APlayerFPSCharacter::Move);
			EnhancedPlayerInput->BindAction(IA_Look, ETriggerEvent::Triggered, this, &APlayerFPSCharacter::Look);
			EnhancedPlayerInput->BindAction(IA_Jump, ETriggerEvent::Triggered, this, &ACharacter::Jump);
			EnhancedPlayerInput->BindAction(IA_Jump, ETriggerEvent::Completed, this, &ACharacter::StopJumping);

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

void APlayerFPSCharacter::OnDeath_Implementation()
{
	AFPSPlayerController* PlayerController = Cast<AFPSPlayerController>(GetController());
	if (PlayerController) {
		
	}
}

void APlayerFPSCharacter::OnTakeDamage_Implementation()
{
	AFPSPlayerController* PlayerController = Cast<AFPSPlayerController>(GetController());
	if (PlayerController) {
		PlayerController->UpdateHealthPercent(HealthComponent->GetHealthPercent());
	}
}

void APlayerFPSCharacter::OnTakeArmorLosing_Implementation()
{
	AFPSPlayerController* PlayerController = Cast<AFPSPlayerController>(GetController());
	if (PlayerController) {
		PlayerController->UpdateArmorPercent(HealthComponent->GetArmorPercent());
	}
}
