// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "Weapon/WeaponBase.h" 
#include "Game/FPSGameModeBase.h"
#include "MultiplayerFPS/MultiplayerFPS.h"
#include "HealthComponent.generated.h"


UCLASS(ClassGroup = (Custom), meta = (BlueprintSpawnableComponent))
class MULTIPLAYERFPS_API UHealthComponent : public UActorComponent
{
	GENERATED_BODY()

public:
	// Sets default values for this component's properties
	UHealthComponent();

protected:
	class AFPSCharacterBase* Player;
	class AController* PlayerController;
		//game mode 
	UPROPERTY()
		class AFPSGameModeBase* GameMode;

	UPROPERTY(Replicated, BlueprintReadOnly, Category = "Character State")
		float Health;
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Character State")
		float MaxHealth = 100.f;
	UPROPERTY(Replicated, BlueprintReadOnly, Category = "Character State")
		float Armor;
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Character State")
		float MaxArmor = 100.f;
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Character State")
		float ArmorAbsorption = 0.5;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Character State")
		class USoundBase* HitSound;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Sounds")
		USoundBase* PainSound;

	bool bIsDead = Health > 0.f ? false : true;

	virtual void BeginPlay() override;
public:

	//Health
	void AddHealth(float Amount) { SetHealth(Health + Amount); }
	void RemoveHealth(float Amount) { SetHealth(Health - Amount); }
	void SetHealth(float NewHealth) { Health = FMath::Clamp(NewHealth, 0.0f, MaxHealth); }
	bool IsDead() const { return Health <= 0.0f; }
	float GetHealth() { return Health; }

	// Armor
	void AddArmor(float Amount) { SetArmor(Armor + Amount); }
	void SetArmor(float Amount) { Armor = FMath::Clamp(Amount, 0.0f, MaxArmor); }
	float ArmorAbsorbDamage(float& Damage);
	float GetArmor() { return Armor; }

	//Damage
	void ApplyDamage(float Damage,class AFPSCharacterBase* DamageCauser);

	AFPSGameModeBase* GetGameMode() { return GameMode; }
	void SetGameMode(class AFPSGameModeBase* NewGameMode){GameMode = NewGameMode;}
	void SetCompInfo(class AFPSCharacterBase* NewPlayer);

	FORCEINLINE float GetHealthPercent() const { return Health / 100.f; }
	FORCEINLINE float GetArmorPercent() const { return Armor / 100.f; } 
};
