// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Weapon/WeaponBase.h" 
#include "MultiplayerFPS/MultiplayerFPS.h"
#include "GameFramework/Character.h"
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
		class USoundBase* WeaponChangedSound;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Sounds")
		class USoundBase* SpawnSound;

	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

	UPROPERTY(Replicated, BlueprintReadOnly, Category = "FPS Character")
		float Health;
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "FPS Character")
		float MaxHealth = 100.f;
	UPROPERTY(Replicated, BlueprintReadOnly, Category = "FPS Character")
		float Armor;
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "FPS Character")
		float MaxArmor = 100.f;
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "FPS Character")
		float ArmorAbsorption;

	UPROPERTY(Replicated, BleuprintReadOnly, Category = "FPS Character")
		class UHealthComponent* StateComp;

	UPROPERTY(Replicated, BlueprintReadOnly, Category = "FPS Character")
		TArray<class AWeaponBase*> Weapons;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "FPS Character")
		TArray<TSubclassOf<class AWeaponBase>>WeaponClasses;

	UPROPERTY(Replicated, BlueprintReadOnly, Category = "FPS Character")
		TArray<int32>Ammo;

	UPROPERTY(Replicated, BlueprintReadOnly, Category = "FPS Character")
		class AWeaponBase* Weapon;

	int32 WeaponIndex = INDEX_NONE;

	UFUNCTION(Server, Reliable)
		void ServerCycleWeapons(int32 Direction);
	UFUNCTION(Server, Reliable)
		void ServerEquipWeapon(EWeaponType WeaponType);

	bool EquipWeapon(EWeaponType WeaponType, bool bPlaySound = true);

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "FPS Character")
		class USoundBase* HitSound;

public:
	// Called to bind functionality to input
	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;

	//Health
	void AddHealth(float Amount) { SetHealth(Health + Amount); }
	void RemoveHealth(float Amount) { SetHealth(Health - Amount); }
	void SetHealth(float NewHealth) { Health = FMath::Clamp(NewHealth, 0.0f, MaxHealth); }
	bool IsDead() const { return Health == 0.0f; }
	float GetHealth() { return Health; }
	// Armor
	void AddArmor(float Amount) { SetArmor(Armor + Amount); }
	void SetArmor(float Amount) { Armor = FMath::Clamp(Amount, 0.0f, MaxArmor); }
	void ArmorAbsorbDamage(float& Damage);
	float GetArmor() { return Armor; }
	//Ammo
	UFUNCTION(BlueprintCallable, Category = "FPS Character")
		int32 GetWeaponAmmo(const EAmmoType AmmoType)const { return Weapon != nullptr ? Ammo[ENUM_TO_INT32(Weapon->GetAmmoType())] : 0; };

	void AddAmmo(EAmmoType AmmoType, int32 Amount) { SetAmmo(AmmoType, GetAmmo(AmmoType) + Amount); }
	void ConsumeAmmo(EAmmoType AmmoType, int32 Amount) { SetAmmo(AmmoType, GetAmmo(AmmoType) - Amount); }

	int32 GetAmmo(EAmmoType AmmoType) { return Ammo[ENUM_TO_INT32(AmmoType)]; }
	void SetAmmo(EAmmoType AmmoType, int32 Amount) { Ammo[ENUM_TO_INT32(AmmoType)] = FMath::Max(0, Amount); }

	
	//RPCs
	UFUNCTION(NetMulticast, Unreliable)
		void MulticastPlayAnimMontage(UAnimMontage* AnimMontage);
	UFUNCTION(Client, Unreliable)
		void ClientPlaySound(USoundBase* Sound);

	//Weapon
	void AddWeapon(EWeaponType WeaponType);

	//Damage
	void ApplyDamage(float Damage, AFPSCharacterBase* DamageCauser);
	
};
