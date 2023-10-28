// Fill out your copyright notice in the Description page of Project Settings.


#include "Character/CharacterComponent/LookAtComponent.h"
#include "Character/Player/PlayerFPSCharacter.h"
#include "Pickups/PickupBase.h"
#include "Kismet/KismetMathLibrary.h"
#include "Kismet/GameplayStatics.h"
#include "Engine/World.h"

// Sets default values for this component's properties
ULookAtComponent::ULookAtComponent()
{
	// Set this component to be initialized when the game starts, and to be ticked every frame.  You can turn these features
	// off to improve performance if you don't need them.
	PrimaryComponentTick.bCanEverTick = true;

}

// Called every frame
void ULookAtComponent::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);

	APlayerFPSCharacter* PlayerCharacter = Cast<APlayerFPSCharacter>(GetOwner());
	const FVector Start = PlayerCharacter->GetFollowCamera();
	const FVector ViewDirection = PlayerCharacter->GetCameraDirection();
	const FVector End = Start + ViewDirection * PickupArm;

	FHitResult Hit(ForceInit);
	FCollisionQueryParams TraceParams("Pick Trace", false, PlayerCharacter);
	GetWorld()->LineTraceSingleByChannel(Hit, Start, End, ECC_Visibility, TraceParams);

	APickupBase* HitPickup = Cast<APickupBase>(Hit.GetActor());
	if (HitPickup != nullptr) {
		TargetPickup = HitPickup;
		const FString Values = FString::Printf(TEXT("%s"), *TargetPickup->GetPickupName());
		DrawDebugString(GetWorld(), TargetPickup->GetActorLocation(), Values, nullptr, FColor::Red, 0.f, true);
	}
	else
	{
		TargetPickup = nullptr;
	}

}

