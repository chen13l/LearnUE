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

	// Called every frame
	virtual void Tick(float DeltaTime) override;

protected:
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Weapon")
		FName WeaponName;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Weapon")
		EWeaponType WeaponType;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Weapon")
		EWeaponFireMode FireMode;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Weapon")
		EAmmoType AmmoType;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Weapon")
		float HitScan;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Weapon")
		float Damage;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Weapon")
		float FireRate;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Mesh")
		class USkeletalMeshComponent* Mesh;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Anims")
		class UAnimMontage* FireAnim;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Sounds")
		class USoundBase* NoAmmoSound;

	UPROPERTY()
		class APlayerFPSCharacter* Character;

	FTimerHandle FireTimer;
	bool bWantsFire;


	virtual	void SetOwner(AActor* NewOwner) override;

	virtual void StartFire();

	void FireHitScan(FVector FireLocation, FVector FireDirection);

	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:

	UFUNCTION(Server, Reliable)
		void ServerStartFire();

	UFUNCTION(Server, Reliable)
		void ServerStopFire();

	EAmmoType GetAmmoType() const { return AmmoType; }
};
