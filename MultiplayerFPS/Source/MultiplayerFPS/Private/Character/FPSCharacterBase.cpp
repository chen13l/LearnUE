// Fill out your copyright notice in the Description page of Project Settings.


#include "Character/FPSCharacterBase.h"
#include "Weapon/WeaponBase.h" 
#include "MultiplayerFPS\MultiplayerFPS.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "Kismet/GameplayStatics.h"
#include "Net/UnrealNetwork.h"

// Sets default values
AFPSCharacterBase::AFPSCharacterBase()
{
	// Set this character to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = false;

	GetCharacterMovement()->MaxWalkSpeed = 800.f;
	GetCharacterMovement()->JumpZVelocity = 600.f;

	StateComp = CreateDefaultSubonject<UHealthComponent>(TEXT("StateComponent"));
}

// Called when the game starts or when spawned
void AFPSCharacterBase::BeginPlay()
{
	Super::BeginPlay();

	if (SpawnSound) {
		UGameplayStatics::PlaySound2D(GetWorld(), SpawnSound);
	}
	//only the authority can go from this point on
	if (!HasAuthority()) {
		return;
	}

}

void AFPSCharacterBase::ServerCycleWeapons_Implementation(int32 Direction)
{
	/*when Direction == 1 the search will go forward
	and do the next weapon functionality
	when Direction == -1 the search will go backwards
	and do the previous weapon functionality*/
	const int32 WeaponCount = Weapons.Num();1

	//cycle through each index in the weapons array and try to equip it 
	const int32 StartWeaponIndex = GET_CIRCULAR_ARRAY_INDEX(WeaponIndex + Direction, WeaponCount);

	for (int32 i = StartWeaponIndex; i != WeaponIndex; i = GET_CIRCULAR_ARRAY_INDEX(i + Direction, WeaponCount)) {
		//abort the for loop if the equip was successful
		if (EquipWeapon((EWeaponType)i)) {
			break;
		}
	}
}

void AFPSCharacterBase::ServerEquipWeapon_Implementation(EWeaponType WeaponType)
{
	EquipWeapon(WeaponType);
}


// Called to bind functionality to input
void AFPSCharacterBase::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);
}



void AFPSCharacterBase::ClientPlaySound_Implementation(USoundBase* Sound)
{
	UGameplayStatics::PlaySound2D(GetWorld(), Sound);
}

void AFPSCharacterBase::MulticastPlayAnimMontage_Implementation(UAnimMontage* AnimMontage)
{
	PlayAnimMontage(AnimMontage);
}


