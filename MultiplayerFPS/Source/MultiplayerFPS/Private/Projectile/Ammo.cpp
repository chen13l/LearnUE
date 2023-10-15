// Fill out your copyright notice in the Description page of Project Settings.


#include "Projectile/Ammo.h"
#include "Character\CharacterComponent\HealthComponent.h"
#include "Character\Player\PlayerFPSCharacter.h"
#include "Character/Player/FPSPlayerController.h"
#include "Components/SphereComponent.h"
#include "GameFramework/ProjectileMovementComponent.h"

#include "Kismet/KismetSystemLibrary.h"
#include "Kismet/GameplayStatics.h"

// Sets default values
AAmmo::AAmmo()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	SphereComponent = CreateDefaultSubobject<USphereComponent>(TEXT("Sphere Colliision"));
	SphereComponent->SetSphereRadius(5.f);
	SphereComponent->SetCollisionProfileName("Ammo");
	//simulate generate hit event
	SphereComponent->SetSimulatePhysics(true);
	SphereComponent->SetNotifyRigidBodyCollision(true);
	
	SphereComponent->OnComponentHit.AddDynamic(this, &AAmmo::OnHit);

	RootComponent = SphereComponent;

	AmmoMovementComp = CreateDefaultSubobject<UProjectileMovementComponent>(TEXT("Ammo Movement"));
	AmmoMovementComp->InitialSpeed = 4000.f;
}

// Called when the game starts or when spawned
void AAmmo::BeginPlay()
{
	Super::BeginPlay();
	
	SetLifeSpan(15.f);
}

// Called every frame
void AAmmo::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

void AAmmo::OnHit(UPrimitiveComponent* HitComp, AActor* OtherActor, UPrimitiveComponent* OtherComp, FVector NormalImpulse, const FHitResult& Hit)
{
	if (HitSound && NormalImpulse.Size() >= 600.f) {
		UGameplayStatics::PlaySoundAtLocation(this,
			HitSound,
			GetActorLocation(),
			1.f,
			1.f,
			0.f,
			HitSoundAttenuation);
	}

	APlayerFPSCharacter* Player = Cast<APlayerFPSCharacter>(OtherActor);

	if (Player) {
		UHealthComponent* HealthComp = Player->FindComponentByClass<UHealthComponent>();
		if (HealthComp) {
			HealthComp->ApplyDamage(Damage, AmmoOwner);
			AFPSPlayerController* PlayerController = Cast<AFPSPlayerController>(Player->GetController());
			if (PlayerController != nullptr) {
				PlayerController->UpdateHealthPercent(Player->GetHealthComp()->GetHealthPercent());
				PlayerController->UpdateArmorPercent(Player->GetHealthComp()->GetArmorPercent());
			}
		}
		if (HitParticles) {
			UGameplayStatics::SpawnEmitterAtLocation(GetWorld(), HitParticles, GetActorTransform());
		}

		if (DamageSound) {
			UGameplayStatics::PlaySound2D(GetWorld(), DamageSound);
		}
		
		Destroy();
	}
}

