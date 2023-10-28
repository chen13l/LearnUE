// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "MultiplayerFPS/MultiplayerFPS.h"
#include "Character/CharacterComponent/HealthComponent.h"
#include "Character//CharacterComponent/WeaponComponent.h"
#include "GameFramework/Character.h"
#include "Kismet/GameplayStatics.h"
#include "FPSCharacterBase.generated.h"

UCLASS()
class MULTIPLAYERFPS_API AFPSCharacterBase : public ACharacter
{
	GENERATED_BODY()

public:
	// Sets default values for this character's properties
	AFPSCharacterBase();

protected:

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Sounds")
		class USoundBase* SpawnSound;


	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Sounds")
		USoundBase* LandSound;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "FPS Character")
		UHealthComponent* StateComp;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "FPS Character")
		UWeaponComponent* WeaponComp;

	// Called when the game starts or when spawned
	virtual void BeginPlay() override;
	virtual void EndPlay(const EEndPlayReason::Type EndPlayReason) override;

	UFUNCTION(Server, Reliable)
		void ServerCycleWeapons(int32 Direction);
	UFUNCTION(Server, Reliable)
		void ServerEquipWeapon(EWeaponType WeaponType);

	virtual void FellOutOfWorld(const UDamageType& DmgType) override;

	//
	virtual void Landed(const FHitResult& Hit) override;

public:

	// Called to bind functionality to input
	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;

	UFUNCTION(BlueprintCallable)
		UHealthComponent* GetHealthComp() const { return StateComp; }
	UFUNCTION(BlueprintCallable)
		UWeaponComponent* GetWeaponComp() const { return WeaponComp; }

	//RPCs
	UFUNCTION(NetMulticast, Unreliable)
		void MulticastPlayAnimMontage(UAnimMontage* AnimMontage);
	UFUNCTION(Client, Unreliable)
		void ClientPlaySound(USoundBase* Sound);
	UFUNCTION(NetMulticast, Unreliable)
		void MulticastPlayFireSound(USoundBase* FireSound, USoundAttenuation* FireSoundeAttenuation);
};
