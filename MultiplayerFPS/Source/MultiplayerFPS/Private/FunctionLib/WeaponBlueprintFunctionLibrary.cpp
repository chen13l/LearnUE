// Fill out your copyright notice in the Description page of Project Settings.


#include "FunctionLib/WeaponBlueprintFunctionLibrary.h"
#include "Engine\World.h"
#include "CollisionQueryParams.h"
#include "DrawDebugHelpers.h"

bool UWeaponBlueprintFunctionLibrary::CanSeeActor(const UWorld* World,
	FVector Location,
	const AActor* TargetActor,
	TArray<const AActor*>IgnoreActors = TArray<const AActor*>()) {
	if (TargetActor == nullptr) {
		return false;
	}
	//Store the result of trace
	FHitResult Hit;

	//Where the trace start and end 
	FVector Start = Location;
	FVector End = TargetActor->GetActorLocation();

	//the trace channel
	ECollisionChannel Channel = ECollisionChannel::ECC_Visibility;

	//Structure that defines parameters passed into collision function
	FCollisionQueryParams Params;
	Params.AddIgnoredActors(IgnoreActors);

	//Rotation of the shape used in sweep trace
	FQuat Rotation = FQuat::Identity;
	//shape of the object used in sweep trace
	FCollisionShape Shape = FCollisionShape::MakeBox(FVector(20.f, 20.f, 20.f));

	World->SweepSingleByChannel(Hit, Start, End, Rotation, Channel, Shape, Params);

	DrawDebugLine(World, Start, End, FColor::Red);

	return !Hit.bBlockingHit;
}