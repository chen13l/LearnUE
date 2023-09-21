// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/PlayerState.h"
#include "AbilitySystemInterface.h"
#include "Character/Abilities/AttributeSets/CharacterAttributeSetBase.h"
#include "Character/Abilities/CharacterAbilitySystemComponent.h"
#include "GameplayEffectTypes.h"
#include "DemoPlayerState.generated.h"

/**
 * inherited IAbilitySystemInterface, each player state wants to have a referenece to
 * its ASC
 */
class UAbilitySystemComponent;
class UCharacterAttributeSetBase;
class UCharacterAbilitySystemComponent;

UCLASS()
class DEMO_API ADemoPlayerState : public APlayerState, public IAbilitySystemInterface
{
	GENERATED_BODY()
public:
	ADemoPlayerState();

	UAbilitySystemComponent* GetAbilitySystemComponent() const override;

	UCharacterAttributeSetBase* GetAttributeSetBase() const;

	UFUNCTION(BlueprintCallable, Category = "Demo|DemoPlayerState")
		bool IsAlive() const;

	UFUNCTION(BlueprintCallable, Category = "Demo|DemoPlayerState|UI")
		void ShowAbilityConfirmCancelText(bool ShowText);

	UFUNCTION(BlueprintCallable, Category = "Demo|DemoPlayerState|Attributes")
		float GetHealth() const;

	UFUNCTION(BlueprintCallable, Category = "Demo|DemoPlayerState|Attributes")
		float GetMaxHealth() const;

	UFUNCTION(BlueprintCallable, Category = "Demo|DemoPlayerState|Attributes")
		float GetMana()const;

	UFUNCTION(BlueprintCallable, Category = "Demo|DemoPlayerState|Attributes")
		float GetMaxMana()const;

	UFUNCTION(BlueprintCallable, Category = "Demo|DemoPlayerState|Attributes")
		float GetCharacterLevel()const;

protected:
	UPROPERTY()
		UCharacterAbilitySystemComponent* AbilitySystemComponent;

	UPROPERTY()
		UCharacterAttributeSetBase* AttributeSetBase;

	FGameplayTag DeadTag;

	FDelegateHandle HealthChangedDelegateHandle;
	FDelegateHandle MaxHealthChangedDelegateHandle;
	FDelegateHandle ManaChangedDelegateHandle;
	FDelegateHandle MaxManaChangedDelegateHandle;
	FDelegateHandle CharacterLevelChangedDelegateHandle;

	virtual void BeginPlay() override;

	//call back function when attribute changed
	//basically used to update the UI 
	virtual void HealthChanged(const FOnAttributeChangeData& Data);
	virtual void MaxHealthChanged(const FOnAttributeChangeData& Data);
	virtual void ManaChanged(const FOnAttributeChangeData& Data);
	virtual void MaxManaChanged(const FOnAttributeChangeData& Data);
	virtual void CharacterLevelChanged(const FOnAttributeChangeData& Data);

	//call back function whenever the stun tag change
	virtual void StunTagChanged(const FGameplayTag CallbackTag, int32 NewCount);
};
