// Fill out your copyright notice in the Description page of Project Settings.


#include "Weapon/WeaponBase.h"
#include "Character/Player/PlayerFPSCharacter.h"
#include "Projectile/Ammo.h"
#include "Components/SkeletalMeshComponent.h"
#include "GameFramework/ProjectileMovementComponent.h"

// Sets default values
AWeaponBase::AWeaponBase()
{
	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = false;

	Mesh = CreateDefaultSubobject<USkeletalMeshComponent>(TEXT("Mesh"));
	RootComponent = Mesh;

	AmmoFrom = CreateDefaultSubobject<USceneComponent>(TEXT("AmmoFrom"));
	AmmoFrom->SetupAttachment(RootComponent);

	bReplicates = true;
}

void AWeaponBase::SetOwner(AActor* NewOwner)
{
	Super::SetOwner(NewOwner);

	Character = Cast<APlayerFPSCharacter>(NewOwner);
}

void AWeaponBase::StartFire()
{
	/*abort if the trigger is no longer down or if there is still time remaining in the
	FireTimer*/
	if (!bWantsFire || GetWorldTimerManager().GetTimerRemaining(FireTimer) > 0.f) {
		return;
	}

	//if there is no ammo, play the no ammo sound on the owning clent 
	if (Character->GetWeaponAmmo(AmmoType) == 0) {
		if (NoAmmoSound) {
			Character->ClientPlaySound(NoAmmoSound);
		}
		return;
	}

	//consume the ammo
	Character->ConsumeAmmo(AmmoType, 1);

	//fire the hitscan using the camera location and direction
	const FVector FireLocation = Character->GetFollowCamera();
	const FVector FireDirection = Character->GetCameraDirection();

	FireHitScan(FireLocation, FireDirection);

	//player the fire anim montage in all of the instances of the owning character
	if (FireAnim) {
		Character->MulticastPlayAnimMontage(FireAnim);
	}

	SpawnAmmo();

	//schedule the FireTimer depending on the value of the FireMode
	if (FireMode == EWeaponFireMode::Automatic && bWantsFire) {
		GetWorldTimerManager().SetTimer(FireTimer, this, &AWeaponBase::StartFire, FireRate);
	}
	else if (FireMode == EWeaponFireMode::Single) {
		GetWorldTimerManager().SetTimer(FireTimer, FireRate, false);
	}

}

void AWeaponBase::FireHitScan(FVector FireLocation, FVector FireDirection)
{
	/*perform a line trace from the firelocation and towards the FireDirection
	multiplied by the HitscanRange, that ignores the owning character*/

	FHitResult Hit(ForceInit);
	FCollisionQueryParams TraceParams("Fire Trae", false, Character);

	const FVector Start = FireLocation;
	const FVector End = Start + FireDirection * HitScanRange;

	GetWorld()->LineTraceSingleByChannel(Hit, Start, End, ECC_Visibility, TraceParams);

	//apply damage if the hit actor is a PlayerFPSCharacter
	AFPSCharacterBase* HitCharacter = Cast<AFPSCharacterBase>(Hit.GetActor());

	if (HitCharacter) {
		HitCharacter->ApplyDamage(Damage, Character);
	}
}

void AWeaponBase::OnPressedFire()
{
	ServerStartFire();
}

void AWeaponBase::OnReleasedFire()
{
	ServerStopFire();
}

void AWeaponBase::ServerStartFire_Implementation()
{
	bWantsFire = true;

	StartFire();
}

void AWeaponBase::ServerStopFire_Implementation()
{
	bWantsFire = false;
}

void AWeaponBase::SpawnAmmo_Implementation() {

	const FVector FireDirection = Character->GetCameraDirection();

	//spawn ammo
	float SpawnDistance = 20.f;
	const FVector SpawnLocation = AmmoFrom->GetComponentLocation() + (FireDirection * SpawnDistance);
	FTransform SpawnTransform(AmmoFrom->GetComponentRotation(), SpawnLocation);
	AAmmo* Projectile = GetWorld()->SpawnActorDeferred<AAmmo>(AmmoClass, SpawnTransform);
	Projectile->GetAmmoMovementComponent()->InitialSpeed = 9999.9f;
	Projectile->FinishSpawning(SpawnTransform);
}