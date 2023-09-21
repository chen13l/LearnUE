// Fill out your copyright notice in the Description page of Project Settings.


#include "SuperSideScroller_Player.h"
#include "PlayerProjectile.h"
#include "Components/InputComponent.h"
#include "Components/SphereComponent.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "Engine/World.h"
#include "TimerManager.h"
#include "Animation/AnimInstance.h"
#include "EnhancedInputComponent.h"
#include "EnhancedInputSubsystems.h"

ASuperSideScroller_Player::ASuperSideScroller_Player()
{
	//Set sprinting to false by default.
	bIsSprinting = false;

	//Set our Max Walk Speed to 300.0f
	GetCharacterMovement()->MaxWalkSpeed = 300.0f;
}

void ASuperSideScroller_Player::SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent)
{
	//Not always necessary, but good practice to call the function in the base class with Super.
	Super::SetupPlayerInputComponent(PlayerInputComponent);

	UEnhancedInputComponent* EhancedInput = Cast<UEnhancedInputComponent>(PlayerInputComponent);
	if (EhancedInput) {
		APlayerController* PlayerController = Cast<APlayerController>(Controller);
		UEnhancedInputLocalPlayerSubsystem* SubSystem = 
			ULocalPlayer::GetSubsystem<UEnhancedInputLocalPlayerSubsystem>(PlayerController->GetLocalPlayer());
		if (SubSystem) {
			SubSystem->AddMappingContext(IC_Character, 0);
			EhancedInput->BindAction(IA_Sprint, ETriggerEvent::Triggered, this, &ASuperSideScroller_Player::Sprint);
			EhancedInput->BindAction(IA_Sprint, ETriggerEvent::Completed, this, &ASuperSideScroller_Player::StopSprinting);
			EhancedInput->BindAction(IA_Throw, ETriggerEvent::Triggered, this, &ASuperSideScroller_Player::ThrowProjectile);
		}
	}

}

void ASuperSideScroller_Player::Sprint()
{
	//First check the bIsSprinting variable, if !(NOT) sprinting, set the variable and update the max walk speed to 500.0f
	if (!bIsSprinting)
	{
		bIsSprinting = true;
		GetCharacterMovement()->MaxWalkSpeed = bHasPowerupActive ? 900.f : 500.f;
	}
}

void ASuperSideScroller_Player::StopSprinting()
{
	//First check the bIsSprinting variable, if sprinting, set the variable and update the max walk speed to 300.0f
	if (bIsSprinting)
	{
		bIsSprinting = false;
		GetCharacterMovement()->MaxWalkSpeed = bHasPowerupActive ? 500.f : 300.f;
	}
}

void ASuperSideScroller_Player::ThrowProjectile()
{
	if (ThrowMontage)
	{
		bool bIsMontagePlaying = GetMesh()->GetAnimInstance()->Montage_IsPlaying(ThrowMontage);
		if (!bIsMontagePlaying)
		{
			GetMesh()->GetAnimInstance()->Montage_Play(ThrowMontage, 2.0f);
		}
	}
}

void ASuperSideScroller_Player::SpawnProjectile() {
	if (PlayerProjectile) {
		UWorld* World = GetWorld();
		if (World) {
			FActorSpawnParameters SpawnParams;
			SpawnParams.Owner = this;
			const FVector SpawnLocation = this->GetMesh()->GetSocketLocation(FName("ProjectileSocket"));
			const FRotator Rotation = GetActorForwardVector().Rotation();
			APlayerProjectile* Projectile = World->SpawnActor<APlayerProjectile>(PlayerProjectile, SpawnLocation, Rotation, SpawnParams);
		}
	}
}

void ASuperSideScroller_Player::IncrementNumberOfCollectibles(int32 Value) {
	if (Value <= 0) {
		return;
	}
	else {
		NumberOfCollectibles += Value;
		UE_LOG(LogTemp, Warning, TEXT("Number of coins: %d"), NumberOfCollectibles);
	}

}

void ASuperSideScroller_Player::IncreaseMovementPowerup() {
	bHasPowerupActive = true;
	GetCharacterMovement()->MaxWalkSpeed = 500.f;
	GetCharacterMovement()->JumpZVelocity = 1500.f;

	UWorld* World = GetWorld();
	if (World) {
		World->GetTimerManager().SetTimer(PowerupHandle, this, &ASuperSideScroller_Player::EndPowerup, 8.f, false);
	}
}

void ASuperSideScroller_Player::EndPowerup() {
	bHasPowerupActive = false;
	GetCharacterMovement()->MaxWalkSpeed = 300.f;
	GetCharacterMovement()->JumpZVelocity = 1000.f;

	UWorld* World = GetWorld();
	if (World) {
		World->GetTimerManager().ClearTimer(PowerupHandle);
	}
}