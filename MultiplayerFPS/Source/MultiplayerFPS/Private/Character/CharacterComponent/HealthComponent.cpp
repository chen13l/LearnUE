// Fill out your copyright notice in the Description page of Project Settings.


#include "Character/CharacterComponent/HealthComponent.h"
#include "Character/FPSCharacterBase.h"
#include "Weapon/WeaponBase.h" 
#include "Game/FPSGameModeBase.h"
#include "Net/UnrealNetwork.h"
#include "Engine/World.h"
#include "Kismet/GameplayStatics.h"
#include "GameFramework/Controller.h"

// Sets default values for this component's properties
UHealthComponent::UHealthComponent()
{
	// Set this component to be initialized when the game starts, and to be ticked every frame.  You can turn these features
	// off to improve performance if you don't need them.
	PrimaryComponentTick.bCanEverTick = false;

	SetHealth(MaxHealth);
	SetArmor(MaxArmor);
}

void UHealthComponent::BeginPlay()
{
	Super::BeginPlay();
	GameMode = Cast<AFPSGameModeBase>(GetWorld()->GetAuthGameMode());
}

float UHealthComponent::ArmorAbsorbDamage(float& Damage)
{
	//calculate how much damage was absorbed and sbustract that from the armor
	const float AbsorbDamage = Damage * ArmorAbsorption;
	const float RemainingArmmor = FMath::Max(Armor - AbsorbDamage, 0.f);

	SetArmor(RemainingArmmor);

	//Recalculate the damage
	if (!RemainingArmmor) {
		float RemainingDamage = (Damage * (1 - ArmorAbsorption)) - FMath::Min(RemainingArmmor, 0.f);
		return RemainingDamage;
	}
	return 0;
}

void UHealthComponent::ApplyDamage(float Damage, AFPSCharacterBase* DamageCauser)
{
	float RemainingDamage = 0;
	//deduct the armor and the health
	if (Armor > 0) {
		RemainingDamage = ArmorAbsorbDamage(Damage);
		RemoveHealth(RemainingDamage);
	}
	else {
		RemoveHealth(Damage);
	}

	ReceivedDamage.Broadcast(GetHealthPercent(), GetArmorPercent());

	//play the hit sound on the owning client of the damage causer
	if (HitSound != nullptr && DamageCauser != nullptr) {
		DamageCauser->ClientPlaySound(HitSound);
	}

	if (PainSound != nullptr) {
		Player->ClientPlaySound(PainSound);
	}

	if (GetHealth() <= 0.0f) {
		if (GameMode != nullptr && DamageCauser != nullptr)
		{
			GameMode->OnKill(DamageCauser->GetController(), PlayerController);
		}
	}

}

void UHealthComponent::SetCompInfo(AFPSCharacterBase* NewPlayer)
{
	Player = NewPlayer;
	PlayerController =Player->GetController();
}

void UHealthComponent::GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps)const {
	Super::GetLifetimeReplicatedProps(OutLifetimeProps);

	DOREPLIFETIME_CONDITION(UHealthComponent, Health, COND_OwnerOnly);
	DOREPLIFETIME_CONDITION(UHealthComponent, Armor, COND_OwnerOnly);
}

