// Fill out your copyright notice in the Description page of Project Settings.


#include "Character/DemoCharacterBase.h"
#include "Components/CapsuleComponent.h"
#include "GameFramework\CharacterMovementComponent.h"
#include "Character\Abilities\CharacterAbilitySystemComponent.h"
#include "Character\Abilities\AttributeSets\CharacterAttributeSetBase.h"
#include "Character\Abilities\CharacterGameplayAbility.h"

// Sets default values
ADemoCharacterBase::ADemoCharacterBase()
{
 	// Set this character to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	GetCapsuleComponent()->SetCollisionResponseToChannel(ECollisionChannel::ECC_Visibility, ECollisionResponse::ECR_Overlap);

	bAlwaysRelevant = true;

	DeadTag = FGameplayTag::RequestGameplayTag(FName("State.Dead"));
	EffectRemoveOnDeathTag = FGameplayTag::RequestGameplayTag(FName("State.RemoveOnDeath"));
	
}

UAbilitySystemComponent* ADemoCharacterBase::GetAbilitySystemComponent() const {
	return AbilitySystemComponent.Get();
}

bool ADemoCharacterBase::IsAlive()const {
	return GetHealth() > 0.f;
}

int32 ADemoCharacterBase::GetAbilityLevel(DemoAbilityID AbilityID)const {
	return 1;
}

void ADemoCharacterBase::RemoveCharacterAbilities() {

	//check if own the character||if have ASC||if ASC has ability to remove
	if (GetLocalRole() != ROLE_Authority || !AbilitySystemComponent.IsValid() || !AbilitySystemComponent->CharacterAbilitiesGiven) {
		return;
	}

	TArray<FGameplayAbilitySpecHandle> AbilitiesToRemove;
	for (const FGameplayAbilitySpec& Spec : AbilitySystemComponent->GetActivatableAbilities()) {
		if ((Spec.SourceObject == this) && CharacterAbilities.Contains(Spec.Ability->GetClass())) {
			//Spec.Handle is a kind of instance of the ability
			AbilitiesToRemove.Add(Spec.Handle);
		}
	}

	for (int32 i = 0; i < AbilitiesToRemove.Num(); ++i) {
		AbilitySystemComponent->ClearAbility(AbilitiesToRemove[i]);
	}

	AbilitySystemComponent->CharacterAbilitiesGiven = false;
}

float ADemoCharacterBase::GetHealth() const {
	if (AttributeSetBase.IsValid()) {
		return AttributeSetBase->GetHealth();
	}

	return 0.f;
}

float ADemoCharacterBase::GetMana() const {
	if (AttributeSetBase.IsValid()) {
		return AttributeSetBase->GetMana();
	}

	return 0.f;
}


float ADemoCharacterBase::GetMaxHealth() const {
	if (AttributeSetBase.IsValid()) {
		return AttributeSetBase->GetMaxHealth();
	}

	return 0.f;
}


float ADemoCharacterBase::GetMaxMana() const {
	if (AttributeSetBase.IsValid()) {
		return AttributeSetBase->GetMaxMana();
	}

	return 0.f;
}

float ADemoCharacterBase::GetCharacterLevel()const {
	if (AttributeSetBase.IsValid()) {
		return AttributeSetBase->GetLevel();
	}
	return 0.f;
}

// Called when the game starts or when spawned
void ADemoCharacterBase::BeginPlay()
{
	Super::BeginPlay();
	
}

void ADemoCharacterBase::FinishDying() {
	Destroy();
}

void ADemoCharacterBase::Die() {
	RemoveCharacterAbilities();
	//disable colliison of character
	GetCapsuleComponent()->SetCollisionEnabled(ECollisionEnabled::NoCollision);
	GetCharacterMovement()->GravityScale = 0.f;
	GetCharacterMovement()->Velocity = FVector(0);
	//broadcast character die
	OnCharacterDied.Broadcast(this);

	if (AbilitySystemComponent.IsValid()) {
		//cancel all abilities
		AbilitySystemComponent->CancelAbilities();
		//add the effect to remove on the death tag
		FGameplayTagContainer EffectTagsToRemove;
		EffectTagsToRemove.AddTag(EffectRemoveOnDeathTag);
		//count how many effects removed
		int32 NumEffectsEremoved = AbilitySystemComponent->RemoveActiveEffectsWithTags(EffectTagsToRemove);
		//add the tag of dead
		AbilitySystemComponent->AddLooseGameplayTag(DeadTag);
	}
	//play the death animation
	if (DeathMontage) {
		PlayAnimMontage(DeathMontage);
	}
	else
	{
		FinishDying();
	}
}

void ADemoCharacterBase::AddCharacterAbilities() {
	if (GetLocalRole() != ROLE_Authority || !AbilitySystemComponent.IsValid() || AbilitySystemComponent->CharacterAbilitiesGiven) {
		return;
	}
	//iterate through every single capability character gameplay abilities
	for (TSubclassOf<UCharacterGameplayAbility>& StartupAbility : CharacterAbilities) {
		//add the ability to our ability system component
		//GameplayAbiliySpec is a kind of instance of ability 
		AbilitySystemComponent->GiveAbility(
			FGameplayAbilitySpec(StartupAbility,
			GetAbilityLevel(StartupAbility.GetDefaultObject()->AbilityID),
			static_cast<int32>(StartupAbility.GetDefaultObject()->AbilityInputID),
			this)
		);
	}

	AbilitySystemComponent->CharacterAbilitiesGiven = true;
}

void ADemoCharacterBase::InitializeAttributes() {
	if (!AbilitySystemComponent.IsValid()) {
		return;
	}

	if (!DefaultAttributes) {
		//let people know to fill the character's Blueprints
		//because without the default attributes we're not goning to know how to initialize the character
		UE_LOG(LogTemp, Error, TEXT("%s() Missing DefaultAttributes for %s.Please fill in the character's Blueprints."), *FString(__FUNCTION__), *GetName());
		return;
	}

	FGameplayEffectContextHandle EffectContext = AbilitySystemComponent->MakeEffectContext();
	EffectContext.AddSourceObject(this);

	FGameplayEffectSpecHandle NewHandle = AbilitySystemComponent->MakeOutgoingSpec(DefaultAttributes, GetCharacterLevel(), EffectContext);
	//make sure NewHandle is valid
	if (NewHandle.IsValid()) {
		//apply the gameplay effect
		FActiveGameplayEffectHandle ActiveGEHandle = 
			AbilitySystemComponent->ApplyGameplayEffectSpecToTarget(*NewHandle.Data.Get(), AbilitySystemComponent.Get());
	}
}

void ADemoCharacterBase::AddStartupEffects() {
	if (GetLocalRole() != ROLE_Authority || !AbilitySystemComponent.IsValid() || AbilitySystemComponent->StartupEffectsApplied) {
		return;
	}
	FGameplayEffectContextHandle EffectContext = AbilitySystemComponent->MakeEffectContext();
	EffectContext.AddSourceObject(this);
	
	for (TSubclassOf<UGameplayEffect>GameplayEffect : StartupEffects) {
		FGameplayEffectSpecHandle NewHandle = AbilitySystemComponent->MakeOutgoingSpec(GameplayEffect, GetCharacterLevel(), EffectContext);
		//make sure NewHandle is valid
		if (NewHandle.IsValid()) {
			//apply the gameplay effect
			FActiveGameplayEffectHandle ActiveGEHandle =
				AbilitySystemComponent->ApplyGameplayEffectSpecToTarget(*NewHandle.Data.Get(), AbilitySystemComponent.Get());
		}
	}

	AbilitySystemComponent->StartupEffectsApplied = true; 
}

void ADemoCharacterBase::SetHealth(float Health) {
	if (AttributeSetBase.IsValid()) {
		AttributeSetBase->SetHealth(Health);
	}
}

void ADemoCharacterBase::SetMana(float Mana) {
	if (AttributeSetBase.IsValid()) {
		AttributeSetBase->SetMana(Mana);
	}
}
