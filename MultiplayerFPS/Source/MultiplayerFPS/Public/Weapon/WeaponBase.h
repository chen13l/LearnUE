// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "MultiplayerFPS/MultiplayerFPS.h"
#include "WeaponBase.generated.h"


UCLASS()
class MULTIPLAYERFPS_API AWeaponBase : public AActor
{
	GENERATED_BODY()

public:
	// Sets default values for this actor's properties
	AWeaponBase();

protected:
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Weapon")
		class USceneComponent* AmmoFrom;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Weapon")
		FName WeaponName;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Weapon")
		EWeaponType WeaponType;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Weapon")
		EWeaponFireMode FireMode;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Weapon")
		EAmmoType AmmoType;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Weapon")
		float HitScanRange = 9999.9f;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Weapon")
		float Damage = 20.f;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Weapon")
		float FireRate = 1.f;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Mesh")
		class USkeletalMeshComponent* Mesh;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Anims")
		class UAnimMontage* FireAnim;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Sounds")
		class USoundBase* NoAmmoSound;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Ammo")
		TSubclassOf<class AAmmo>AmmoClass;

	UPROPERTY()
		class APlayerFPSCharacter* Character;

	FTimerHandle FireTimer;
	bool bWantsFire;


	virtual	void SetOwner(AActor* NewOwner) override;

	virtual void StartFire();

	void FireHitScan(FVector FireLocation, FVector FireDirection);

	UFUNCTION(Server, Reliable)
		void ServerStartFire();

	UFUNCTION(Server, Reliable)
		void ServerStopFire();

	UFUNCTION(NetMulticast, Unreliable)
		void SpawnAmmo();
public:

	EAmmoType GetAmmoType() const { return AmmoType; }

	UFUNCTION(BlueprintCallable)
		void OnPressedFire();

	UFUNCTION(BlueprintCallable)
		void OnReleasedFire();

};

