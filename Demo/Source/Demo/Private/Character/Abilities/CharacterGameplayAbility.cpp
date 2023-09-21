// Fill out your copyright notice in the Description page of Project Settings.


#include "Character/Abilities/CharacterGameplayAbility.h"
#include "Character\Abilities\CharacterAbilitySystemComponent.h"

UCharacterGameplayAbility::UCharacterGameplayAbility() {
	//change the instancing policy 
	InstancingPolicy = EGameplayAbilityInstancingPolicy::InstancedPerActor;
	//default all abilities to be blocked by the state of being dead or being stuned
	//disable any ability activate whenever dead or stuned
	ActivationBlockedTags.AddTag(FGameplayTag::RequestGameplayTag(FName("State.Dead")));
	ActivationBlockedTags.AddTag(FGameplayTag::RequestGameplayTag(FName("State.Debuff.Stun")));
}

void UCharacterGameplayAbility::OnAvatarSet(const FGameplayAbilityActorInfo* ActorInfo, const FGameplayAbilitySpec& Spec)
{
	Super::OnAvatarSet(ActorInfo, Spec);

	if (ActivateAbilityOnGranted) {
		ActorInfo->AbilitySystemComponent->TryActivateAbility(Spec.Handle, false);
	}
}
