// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "HealthComponent.generated.h"


UCLASS(ClassGroup = (Custom), meta = (BlueprintSpawnableComponent))
class MULTIPLAYERFPS_API UHealthComponent : public UActorComponent
{
	GENERATED_BODY()

public:
	// Sets default values for this component's properties
	UHealthComponent();

protected:
	// Called when the game starts
	virtual void BeginPlay() override;

	UPROPERTY(EditDefaultsOnly, Category = "States")
		float Health = 100.f;
	UPROPERTY(EditDefaultsOnly, Category = "States")
		float Armor = 100.f;

public:
	// Called every frame
	virtual void TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction) override;

	void LoseHealth(float Amount);
	void LoseArmor(float Amount);

	FORCEINLINE float GetHealthPercent() const { return Health / 100.f; }
	FORCEINLINE float GetArmorPercent() const { return Armor / 100.f; }
};
