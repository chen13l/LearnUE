// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Character.h"
#include "GameplayTagContainer.h"
#include "AbilitySystemInterface.h"
#include "Demo/Demo.h"
#include "DemoCharacterBase.generated.h"

DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FCharacterDiedDelegate, ADemoCharacterBase*, DemoCharacterBase);

UCLASS()
class DEMO_API ADemoCharacterBase : public ACharacter,public IAbilitySystemInterface
{
	GENERATED_BODY()

public:
	// Sets default values for this character's properties
	ADemoCharacterBase();

	virtual UAbilitySystemComponent* GetAbilitySystemComponent() const override;

	UPROPERTY(BlueprintAssignable, Category = "Demo|Character")
		FCharacterDiedDelegate OnCharacterDied;
	
	UFUNCTION(BluePrintCallable, Category = "Demo|Character")
		virtual bool IsAlive()const;
	
	UFUNCTION(BlueprintCallable, Category = "Demo|Character")
		virtual int32 GetAbilityLevel(DemoAbilityID AbilityID) const;

	virtual void RemoveCharacterAbilities();

	virtual void Die();

	UFUNCTION(BlueprintCallable, Category = "Demo|Character")
	virtual void FinishDying();

	UFUNCTION(BlueprintCallable, Category = "Demo|Character|Attributes")
		float GetHealth()const;
	UFUNCTION(BlueprintCallable, Category = "Demo|Character|Attributes")
		float GetMaxHealth()const;
	UFUNCTION(BlueprintCallable, Category = "Demo|Character|Attributes")
		float GetMana()const;
	UFUNCTION(BlueprintCallable, Category = "Demo|Character|Attributes")
		float GetMaxMana()const;
	UFUNCTION(BlueprintCallable, Category = "Demo|Character|Attributes")
		float GetCharacterLevel()const;

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

	//keep track of ASC and AttributeSet 
	//bascially pointers to ones in our PlayerState
	TWeakObjectPtr<class UCharacterAbilitySystemComponent> AbilitySystemComponent;
	TWeakObjectPtr<class UCharacterAttributeSetBase> AttributeSetBase;

	FGameplayTag DeadTag;

	FGameplayTag EffectRemoveOnDeathTag;

	UPROPERTY(BlueprintReadOnly, EditAnyWhere, Category = "Demo|Character")
		FText CharacterName;

	UPROPERTY(BlueprintReadWrite, EditAnyWhere, Category = "Demo|Character")
		UAnimMontage* DeathMontage;

	UPROPERTY(BlueprintReadOnly, EditAnyWhere, Category = "Demo|Character")
		TArray<TSubclassOf<class UCharacterGameplayAbility>> CharacterAbilities;

	UPROPERTY(BlueprintReadOnly, EditAnyWhere, Category = "Demo|Character")
		TSubclassOf<class UGameplayEffect>DefaultAttributes;

	UPROPERTY(BlueprintReadOnly, EditAnyWhere, Category = "Demo|Character")
		TArray<TSubclassOf<class UGameplayEffect>>StartupEffects;

	virtual void AddCharacterAbilities();

	virtual void InitializeAttributes();

	virtual void AddStartupEffects();

	virtual void SetHealth(float Health);

	virtual void SetMana(float Mana);

};
