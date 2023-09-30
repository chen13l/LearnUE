// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "Ammo.generated.h"

UCLASS()
class MULTIPLAYERFPS_API AAmmo : public AActor
{
	GENERATED_BODY()

private:
	float Damage = 15.f;

public:
	// Sets default values for this actor's properties
	AAmmo();

protected:
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Ammo")
		class USphereComponent* SphereComponent;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Ammo")
		class UProjectileMovementComponent* AmmoMovementComp;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Ammo")
		class USoundBase* HitSound;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Ammo")
		class USoundAttenuation* HitSoundAttenuation;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Ammo")
		class UParticleSystem* HitParticles;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Ammo")
		class USoundBase* DamageSound;

	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	UFUNCTION(BlueprintCallable)
		void OnHit(UPrimitiveComponent* HitComp,
			AActor* OtherActor,
			UPrimitiveComponent* OtherComp,
			FVector NormalImpulse,
			const FHitResult& Hit);

	FORCEINLINE UProjectileMovementComponent* GetAmmoMovementComponent()const {
		return AmmoMovementComp;
	}
};
