// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Abilities/GameplayAbility.h"
#include "Demo/Demo.h"
#include "CharacterGameplayAbility.generated.h"

/**
 * 
 */
UCLASS()
class DEMO_API UCharacterGameplayAbility : public UGameplayAbility
{
	GENERATED_BODY()
	
public:
	UCharacterGameplayAbility();

	//this ability will basically activate whenever the input is pressed 
	UPROPERTY(BlueprintReadOnly, EditAnyWhere, Category = "Ability")
		DemoAbilityID AbilityInputID = DemoAbilityID::None;
	//reference the ability without it automatically trigger when hit the input key 
	UPROPERTY(BlueprintReadOnly, EditAnyWhere, Category = "Ability")
		DemoAbilityID AbilityID = DemoAbilityID::None;

	UPROPERTY(BlueprintReadWrite, EditAnyWhere, Category = "Ability")
		bool ActivateAbilityOnGranted = false;

	virtual void OnAvatarSet(const FGameplayAbilityActorInfo* ActorInfo, const FGameplayAbilitySpec& Spec)override;
};
