// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Components/SceneComponent.h"
#include "LookAtComponent.generated.h"


UCLASS( ClassGroup=(Custom), meta=(BlueprintSpawnableComponent) )
class MULTIPLAYERFPS_API ULookAtComponent : public USceneComponent
{
	GENERATED_BODY()

public:	
	// Sets default values for this component's properties
	ULookAtComponent();

protected:
	// Called when the game starts
	virtual void BeginPlay() override;

	virtual bool LookAt();

	bool bCanSeeTarget = false;
	AActor* TargetActor;

public:	
	// Called every frame
	virtual void TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction) override;

	FORCEINLINE void SetTarget(AActor* NewTargetActor) { TargetActor = NewTargetActor; }
	FORCEINLINE void CanSeeTarget() { return bCanSeeTarget; }
};