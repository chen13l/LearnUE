// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "PickupBase.generated.h"

UCLASS()
class MULTIPLAYERFPS_API APickupBase : public AActor
{
	GENERATED_BODY()

protected:
	FString PickupsName = "";

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "PickupBase")
		class URotatingMovementComponent* RotatingMovement;

	UPROPERTY(EditDefaultsOnly,BlueprintReadOnly,Category="PickupBase")
		class USkeletalMeshComponent* Mesh;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "pickupBase")
		USoundBase* PickupSound;

	bool bIsPickedUp = false;

	// Sets default values for this actor's properties
	APickupBase();

	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:
	UFUNCTION()
		virtual void OnPickedUp(class AFPSCharacterBase* FpsCharacter);

	UFUNCTION(BlueprintCallable)
		void SetPickupName(const FString& NewName) { PickupsName = NewName; }

	UFUNCTION(BlueprintCallable)
		FString GetPickupName()const {return PickupsName;}

};
