// Fill out your copyright notice in the Description page of Project Settings.


#include "Player/DemoPlayerState.h"
#include "Character\Abilities\AttributeSets\CharacterAttributeSetBase.h"
#include "Character\Abilities\CharacterAbilitySystemComponent.h"

ADemoPlayerState::ADemoPlayerState() {
	AbilitySystemComponent = CreateDefaultSubobject<UCharacterAbilitySystemComponent>(TEXT("AbilitySystemComponent"));
	AbilitySystemComponent->SetIsReplicated(true);
	//set to mixed to only see the cues like visual effect or sound effect
	AbilitySystemComponent->SetReplicationMode(EGameplayEffectReplicationMode::Mixed);

	AttributeSetBase = CreateDefaultSubobject<UCharacterAttributeSetBase>(TEXT("AttributeSetBase"));
	//deafaults to very low for player states and may introduce sone sort of 
	//perceived lag to the Ability System
	NetUpdateFrequency = 100.f;
	
	DeadTag = FGameplayTag::RequestGameplayTag(FName("State.Dead"));
}

UAbilitySystemComponent* ADemoPlayerState::GetAbilitySystemComponent() const
{
	return AbilitySystemComponent;
}

UCharacterAttributeSetBase* ADemoPlayerState::GetAttributeSetBase() const
{
	return AttributeSetBase;
}

bool ADemoPlayerState::IsAlive() const
{
	return GetHealth() > 0.f;
}

void ADemoPlayerState::ShowAbilityConfirmCancelText(bool ShowText)
{
	//implement HUD 

}

float ADemoPlayerState::GetHealth() const
{
	return AttributeSetBase->GetHealth();
}

float ADemoPlayerState::GetMaxHealth() const
{
	return AttributeSetBase->GetMaxHealth();
}

float ADemoPlayerState::GetMana() const
{
	return AttributeSetBase->GetMana();
}

float ADemoPlayerState::GetMaxMana() const
{
	return AttributeSetBase->GetMaxMana();
}

float ADemoPlayerState::GetCharacterLevel() const
{
	return AttributeSetBase->GetLevel();
}

void ADemoPlayerState::BeginPlay()
{
	Super::BeginPlay();

	//set delegate handle
	if (AbilitySystemComponent) {
		 //linking the attribute value changing evnet to these function 
		HealthChangedDelegateHandle = AbilitySystemComponent->GetGameplayAttributeValueChangeDelegate(AttributeSetBase->GetHealthAttribute()).AddUObject(this, &ADemoPlayerState::HealthChanged);
		MaxHealthChangedDelegateHandle = AbilitySystemComponent->GetGameplayAttributeValueChangeDelegate(AttributeSetBase->GetHealthAttribute()).AddUObject(this, &ADemoPlayerState::MaxHealthChanged);
		ManaChangedDelegateHandle = AbilitySystemComponent->GetGameplayAttributeValueChangeDelegate(AttributeSetBase->GetHealthAttribute()).AddUObject(this, &ADemoPlayerState::ManaChanged);
		MaxManaChangedDelegateHandle = AbilitySystemComponent->GetGameplayAttributeValueChangeDelegate(AttributeSetBase->GetHealthAttribute()).AddUObject(this, &ADemoPlayerState::MaxManaChanged);
		CharacterLevelChangedDelegateHandle = AbilitySystemComponent->GetGameplayAttributeValueChangeDelegate(AttributeSetBase->GetHealthAttribute()).AddUObject(this, &ADemoPlayerState::CharacterLevelChanged);

		//get called if the stun debuff get applied to us or removed 
		AbilitySystemComponent->RegisterGameplayTagEvent(FGameplayTag::RequestGameplayTag(FName("State.Debuff.Stun")), EGameplayTagEventType::NewOrRemoved).AddUObject(this, &ADemoPlayerState::StunTagChanged);

	}
}

void ADemoPlayerState::HealthChanged(const FOnAttributeChangeData& Data)
{
	UE_LOG(LogTemp, Warning, TEXT("Health Changed"));
}

void ADemoPlayerState::MaxHealthChanged(const FOnAttributeChangeData& Data)
{
	UE_LOG(LogTemp, Warning, TEXT("Max Health Changed"));
}

void ADemoPlayerState::ManaChanged(const FOnAttributeChangeData& Data)
{
	UE_LOG(LogTemp, Warning, TEXT("Mana Changed"));
}

void ADemoPlayerState::MaxManaChanged(const FOnAttributeChangeData& Data)
{
	UE_LOG(LogTemp, Warning, TEXT("Max Mana Changed"));
}

void ADemoPlayerState::CharacterLevelChanged(const FOnAttributeChangeData& Data)
{
	UE_LOG(LogTemp, Warning, TEXT("Character Level Changed"));
}

void ADemoPlayerState::StunTagChanged(const FGameplayTag CallbackTag, int32 NewCount)
{
	//if stun canacel abilities
	if (NewCount > 0) {
		FGameplayTagContainer AbilityTagsToCancel;
		AbilityTagsToCancel.AddTag(FGameplayTag::RequestGameplayTag(FName("Ability")));

		FGameplayTagContainer AbilityTagsToIgnore;
		AbilityTagsToIgnore.AddTag(FGameplayTag::RequestGameplayTag(FName("Ability.NotCancelByStun")));

		AbilitySystemComponent->CancelAbilities(&AbilityTagsToCancel, &AbilityTagsToIgnore);
	}
}
