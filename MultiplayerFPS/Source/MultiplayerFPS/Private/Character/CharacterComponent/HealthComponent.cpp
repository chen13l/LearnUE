// Fill out your copyright notice in the Description page of Project Settings.


#include "Character/CharacterComponent/HealthComponent.h"
#include "Character\CharacterComponent\StateInterface.h"

// Sets default values for this component's properties
UHealthComponent::UHealthComponent()
{
	// Set this component to be initialized when the game starts, and to be ticked every frame.  You can turn these features
	// off to improve performance if you don't need them.
	PrimaryComponentTick.bCanEverTick = false;

	// ...
}


// Called when the game starts
void UHealthComponent::BeginPlay()
{
	Super::BeginPlay();

	// ...
	
}


// Called every frame
void UHealthComponent::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);

	// ...
}

void UHealthComponent::LoseState(float Amount)
{
	const float AbsorbedDamage = Amount * ArmorAbsorption;
	const float RemainingArmor = Armor - AbsorbedDamage;
	Armor = RemainingArmor;
	if (GetOwner()->Implements<UStateInterface>()) {
		IStateInterface::Execute_OnTakeArmorLosing(GetOwner());
	}
	Amount = (Amount * (1 - ArmorAbsorption)) - FMath::Min(RemainingArmor, 0.0f);

	if (Armor <= 0.f) {
		Armor = 0.f;
		Health -= Amount;
		IStateInterface::Execute_OnTakeDamage(GetOwner());
		if (Health <= 0.f) {
			Health = 0.f;
			IStateInterface::Execute_OnDeath(GetOwner());
		}
	}
}

