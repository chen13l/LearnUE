// Fill out your copyright notice in the Description page of Project Settings.


#include "Character/CharacterComponent/LookAtComponent.h"
#include "FunctionLib\WeaponBlueprintFunctionLibrary.h"
#include "Kismet/KismetMathLibrary.h"

// Sets default values for this component's properties
ULookAtComponent::ULookAtComponent()
{
	// Set this component to be initialized when the game starts, and to be ticked every frame.  You can turn these features
	// off to improve performance if you don't need them.
	PrimaryComponentTick.bCanEverTick = true;

	// ...
}


// Called when the game starts
void ULookAtComponent::BeginPlay()
{
	Super::BeginPlay();

	// ...

}


// Called every frame
void ULookAtComponent::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);

	// ...
	bCanSeeTarget = LookAt();
}

bool ULookAtComponent::LookAt()
{
	if (TargetActor == nullptr) { return false; }
	
	TArray<const AActor*>IgnoreActors = { GetOwner(),TargetActor };
	if (UWeaponBlueprintFunctionLibrary::CanSeeActor(GetWorld(),
		GetComponentLocation(),
		TargetActor,
		IgnoreActors)) {
		
		FVector Start = GetComponentLocation();
		FVector End = TargetActor->GetActorLocation();

		//Caculate the necessary rotation for the actor to face target
		FRotator LookAtRotation = UKismetMathLibrary::FindLookAtRotation(Start, End);

		//set rotation for owner
		GetOwner()->SetActorRotation(LookAtRotation);

		return true;
	}

	return false;
}