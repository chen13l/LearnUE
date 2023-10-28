// Fill out your copyright notice in the Description page of Project Settings.


#include "Character/FPSCharacterBase.h"
#include "Weapon/WeaponBase.h" 
#include "Game/FPSGameModeBase.h"
#include "MultiplayerFPS\MultiplayerFPS.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "Net/UnrealNetwork.h"
#include "..\..\Public\Character\FPSCharacterBase.h"

// Sets default values
AFPSCharacterBase::AFPSCharacterBase()
{
	// Set this character to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = false;

	GetCharacterMovement()->MaxWalkSpeed = 800.f;
	GetCharacterMovement()->JumpZVelocity = 600.f;

	StateComp = CreateDefaultSubobject<UHealthComponent>(TEXT("StateComponent"));
	StateComp->SetCompInfo(this);
	WeaponComp = CreateDefaultSubobject<UWeaponComponent>(TEXT("WeaponComponent"));
	WeaponComp->SetCompInfo(this);
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

	AFPSGameModeBase* GameMode = Cast<AFPSGameModeBase>(GetWorld()->GetAuthGameMode());
}

void AFPSCharacterBase::EndPlay(const EEndPlayReason::Type EndPlayReason) {
	Super::EndPlay(EndPlayReason);

}

// Called to bind functionality to input
void AFPSCharacterBase::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);
}

void AFPSCharacterBase::FellOutOfWorld(const UDamageType& DmgType)
{
	AFPSGameModeBase* GameMode = StateComp->GetGameMode();
	if (GameMode != nullptr) {
		GameMode->OnKill(nullptr, GetController());
	}
}

void AFPSCharacterBase::Landed(const FHitResult& Hit)
{
	Super::Landed(Hit);

	UGameplayStatics::PlaySound2D(GetWorld(), LandSound);
}

void AFPSCharacterBase::ServerCycleWeapons_Implementation(int32 Direction)
{
	WeaponComp->CycleWeapons(Direction);
}

void AFPSCharacterBase::ServerEquipWeapon_Implementation(EWeaponType WeaponType)
{
	/*EquipWeapon(WeaponType);*/
	WeaponComp->EquipWeapon(WeaponType);
}

void AFPSCharacterBase::ClientPlaySound_Implementation(USoundBase* Sound)
{
	UGameplayStatics::PlaySound2D(GetWorld(), Sound);
}

void AFPSCharacterBase::MulticastPlayAnimMontage_Implementation(UAnimMontage* AnimMontage)
{
	PlayAnimMontage(AnimMontage);
}


void AFPSCharacterBase::MulticastPlayFireSound_Implementation(USoundBase* FireSound, USoundAttenuation* FireSoundeAttenuation)
{
	UGameplayStatics::PlaySoundAtLocation(GetWorld(),
		FireSound,
		GetActorLocation(),
		1.f,
		1.f,
		0.f,
		FireSoundeAttenuation
	);
}

