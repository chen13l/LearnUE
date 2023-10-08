// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "MultiplayerFPS/MultiplayerFPS.h"
#include "GameFramework/Character.h"
#include "FPSCharacterBase.generated.h"

UCLASS()
class MULTIPLAYERFPS_API AFPSCharacterBase : public ACharacter
	GENERATED_BODY()

public:
	// Sets default values for this character's properties
	AFPSCharacterBase();

protected:
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Sounds")
		class USoundBase* WeaponChangedSound;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Sounds")
		class USoundBase* SpawnSound;

	UPROPERTY(Replicated, BlueprintReadOnly, Category = "FPS Character")
		class UHealthComponent* StateComp;

	TArry<class AWeaponBase*> Weapons = StateComp->GetWeaponArr();

	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

	UFUNCTION(Server, Reliable)
		void ServerCycleWeapons(int32 Direction);
	UFUNCTION(Server, Reliable)
		void ServerEquipWeapon(EWeaponType WeaponType);

public:
	// Called to bind functionality to input
	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;

	//RPCs
	UFUNCTION(NetMulticast, Unreliable)
		void MulticastPlayAnimMontage(UAnimMontage* AnimMontage);
	UFUNCTION(Client, Unreliable)
		void ClientPlaySound(USoundBase* Sound);
	
};
