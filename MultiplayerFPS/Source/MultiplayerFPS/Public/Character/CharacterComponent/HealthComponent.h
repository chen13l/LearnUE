// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "Weapon/WeaponBase.h" 
#include "MultiplayerFPS/MultiplayerFPS.h"
#include "Character/FPSCharacterBase.h"
#include "HealthComponent.generated.h"


UCLASS(ClassGroup = (Custom), meta = (BlueprintSpawnableComponent))
class MULTIPLAYERFPS_API UHealthComponent : public UActorComponent
{
	GENERATED_BODY()

public:
	// Sets default values for this component's properties
	UHealthComponent();

protected:
	// Called when the game starts
	virtual void BeginPlay() override;

	UPROPERTY(Replicated, BlueprintReadOnly, Category = "Character State")
		float Health;
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Character State")
		float MaxHealth = 100.f;
	UPROPERTY(Replicated, BlueprintReadOnly, Category = "Character State")
		float Armor;
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Character State")
		float MaxArmor = 100.f;
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Character State")
		float ArmorAbsorption;

	UPROPERTY(Replicated, BlueprintReadOnly, Category = "Character State")
		TArray<int32>Ammo;

	UPROPERTY(Replicated, BlueprintReadOnly, Category = "Character State")
		class AWeaponBase* Weapon;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Character State")
		float ArmorAbsorption = 0.5;

public:
	// Called every frame
	virtual void TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction) override;

	void LoseState(float Amount);

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
	UFUNCTION(BlueprintCallable, Category = "Character State")
		int32 GetWeaponAmmo(const EAmmoType AmmoType)const { return Weapon != nullptr ? Ammo[ENUM_TO_INT32(Weapon->GetAmmoType())] : 0; };

	void AddAmmo(EAmmoType AmmoType, int32 Amount) { SetAmmo(AmmoType, GetAmmo(AmmoType) + Amount); }
	void ConsumeAmmo(EAmmoType AmmoType, int32 Amount) { SetAmmo(AmmoType, GetAmmo(AmmoType) - Amount); }

	int32 GetAmmo(EAmmoType AmmoType) { return Ammo[ENUM_TO_INT32(AmmoType)]; }
	void SetAmmo(EAmmoType AmmoType, int32 Amount) { Ammo[ENUM_TO_INT32(AmmoType)] = FMath::Max(0, Amount); }

	//Damage
	void ApplyDamage(float Damage, AFPSCharacterBase* DamageCauser);

	FORCEINLINE float GetHealthPercent() const { return Health / 100.f; }
	FORCEINLINE float GetArmorPercent() const { return Armor / 100.f; }
};
