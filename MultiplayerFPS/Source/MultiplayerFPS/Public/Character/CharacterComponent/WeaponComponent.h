// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "MultiplayerFPS/MultiplayerFPS.h"
#include "Weapon/WeaponBase.h"
#include "Projectile/Ammo.h"
#include "WeaponComponent.generated.h"


UCLASS(ClassGroup = (Custom), meta = (BlueprintSpawnableComponent))
class MULTIPLAYERFPS_API UWeaponComponent : public UActorComponent
{
	GENERATED_BODY()

public:
	// Sets default values for this component's properties
	UWeaponComponent();

protected:
	class AFPSCharacterBase* OwnerCharacter;
	bool bShouldWeaponChange = false;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Weapon State")
		class USoundBase* WeaponChangedSound;

	//Weapon
	UPROPERTY(Replicated, BlueprintReadOnly, Category = "Weapon State")
		AWeaponBase* Weapon;

	UPROPERTY(Replicated, BlueprintReadOnly, Category = "Weapon State")
		TArray<AWeaponBase*> Weapons;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Weapon State")
		TArray<TSubclassOf<AWeaponBase>>WeaponClasses;

	UPROPERTY(Replicated, BlueprintReadOnly, Category = "Weapon State")
		TArray<int32>Ammo;

	// Called when the game starts
	virtual void BeginPlay() override;
	virtual void EndPlay(const EEndPlayReason::Type EndPlayReason) override;
public:
	virtual void SetCompInfo(class AFPSCharacterBase* NewOwner) { OwnerCharacter = NewOwner; }

	//weapon
	void AddWeapon(EWeaponType WeaponType);
	int32 WeaponIndex = INDEX_NONE;
	void EquipWeapon(EWeaponType WeaponType, bool  bPlaySound = true);
	void CycleWeapons(int32 Direction);
		AWeaponBase* GetEquipingWeapon()const { return Weapon; }
		UFUNCTION(BlueprintCallable)
		bool HasWeapon()const {return Weapon!=nullptr; }

	//Ammo
	UFUNCTION(BlueprintCallable)
		int32 GetAmmo(const EAmmoType AmmoType)const { return Ammo[ENUM_TO_INT32(AmmoType)]; };
	void AddAmmo(EAmmoType AmmoType, int32 Amount) { SetAmmo(AmmoType, GetAmmo(AmmoType) + Amount); }
	void ConsumeAmmo(EAmmoType AmmoType, int32 Amount) { SetAmmo(AmmoType, GetAmmo(AmmoType) - Amount); }
	void SetAmmo(EAmmoType AmmoType, int32 Amount) { Ammo[ENUM_TO_INT32(AmmoType)] = FMath::Max(0, Amount); }

};
