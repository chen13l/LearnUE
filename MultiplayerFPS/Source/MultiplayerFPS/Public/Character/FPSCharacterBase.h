// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
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

	UPROPERTY(Replicated, BlueprintReadOnly, Category = "FPS Character")
		TArray<class AWeaponBase*> Weapons;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "FPS Character")
		TArray<TSubclassOf<class AWeaponBase>>WeaponClasses;

	UPROPERTY(Replicated, BlueprintReadOnly, Category = "FPS Character")
		TArray<int32>Ammo;

	UPROPERTY(Replicated, BlueprintReadOnly, Category = "FPS Character")
		class AWeapon* Weapon;

	int32 WeaponIndex = INDEX_NONE;

public:
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	// Called to bind functionality to input
	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;


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

};
