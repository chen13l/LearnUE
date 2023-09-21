// Fill out your copyright notice in the Description page of Project Settings.


#include "Character/Player/DemoPlayerCharacter.h"
#include "AI\PlayerAIController.h"
#include "Player/DemoPlayerState.h"
#include "Player/DemoPlayerController.h"
#include "Character/Abilities/CharacterAbilitySystemComponent.h"
#include "GameFramework/SpringArmComponent.h"
#include "Camera/CameraComponent.h"
#include "Components\CapsuleComponent.h"
#include "EnhancedInputComponent.h"
#include "EnhancedInputSubsystems.h"

//do multiple similar thing because have discrepancies between the server and client

ADemoPlayerCharacter::ADemoPlayerCharacter() {
	CameraBoom = CreateDefaultSubobject<USpringArmComponent>(TEXT("CameraBoom"));
	CameraBoom->SetupAttachment(RootComponent);
	CameraBoom->bUsePawnControlRotation = true;
	CameraBoom->SetRelativeLocation(FVector(0.f, 0.f, 70.f));

	FollowCamera = CreateDefaultSubobject<UCameraComponent>(TEXT("FollowCamera"));
	FollowCamera->SetupAttachment(CameraBoom);
	/*	FieldOfView是ucameraccomponent中的一个公共浮动变量，它在透视模式下表示水平视野(以度为单位)(在正交模式下忽略)。*/
	FollowCamera->FieldOfView = 80.f;

	//该组件将忽略与 ECC_Camera 通道相关的所有碰撞事件。
	GetCapsuleComponent()->SetCollisionResponseToChannel(ECC_Camera, ECR_Ignore);

	/*EVisibilityBasedAnimTickOption是c++中的一个枚举，它定义了虚幻引擎中USkinnedMeshComponent类的
	visisibilitybasedanimtickoption属性。
	此属性用于根据组件是否渲染或使用蒙太奇来设置tick动画频率选项。
	该属性的默认值可以在INI文件中更改，并且主要与性能有关*/
	GetMesh()->VisibilityBasedAnimTickOption = EVisibilityBasedAnimTickOption::AlwaysTickPoseAndRefreshBones;
	//用于控制启用了哪种类型的碰撞
	GetMesh()->SetCollisionEnabled(ECollisionEnabled::NoCollision);
	//用于设置物体的碰撞配置文件名称。后有布尔值参数默认true，指示是否更新与该组件相关的重叠检测 
	GetMesh()->SetCollisionProfileName(FName("NoCollision"));

	AIControllerClass = APlayerAIController::StaticClass();

	DeadTag = FGameplayTag::RequestGameplayTag(FName("State.Dead"));
}

//called on server
void ADemoPlayerCharacter::PossessedBy(AController* NewController)
{
	Super::PossessedBy(NewController);

	ADemoPlayerState* PS = GetPlayerState<ADemoPlayerState>();
	if (PS) {
		InitializeStartingValues(PS);

		AddStartupEffects();

		AddCharacterAbilities();
	}
}

USpringArmComponent* ADemoPlayerCharacter::GetCameraBoom()
{
	return CameraBoom;
}

UCameraComponent* ADemoPlayerCharacter::GetFollowCamera()
{
	return FollowCamera;
}

float ADemoPlayerCharacter::GetStartingCameraBoomArmLength()
{
	return StartingCameraBoomArmLength;
}

FVector ADemoPlayerCharacter::GetStartingCameraBoomLocation()
{
	return StartingCameraBoomLocation;
}

void ADemoPlayerCharacter::BeginPlay()
{
	Super::BeginPlay();

	StartingCameraBoomArmLength = CameraBoom->TargetArmLength;
	StartingCameraBoomLocation = CameraBoom->GetRelativeLocation();
}

//called on client
void ADemoPlayerCharacter::OnRep_PlayerState()
{
	Super::OnRep_PlayerState();

	ADemoPlayerState* PS = GetPlayerState<ADemoPlayerState>();
	if (PS) {
		InitializeStartingValues(PS);

		BindASCInput();
	}
}



void ADemoPlayerCharacter::BindASCInput()
{
	if (!ASCInputBound && AbilitySystemComponent.IsValid() && IsValid(InputComponent)) {
		//telling ASC when confirm/cancel target map confirm enum item, 
		//the rest will automatically bind to ASC(for example,we add a new item "fireball" in enum,it will automatically bind to ASC
		AbilitySystemComponent->BindAbilityActivationToInputComponent(InputComponent,
			FGameplayAbilityInputBinds(
				FString("ConfirmTarget"),
				FString("CancelTarget"),
				FTopLevelAssetPath("/Script/Demo.DemoAbilityID"),
				static_cast<int32>(DemoAbilityID::Confirm),
				static_cast<int32>(DemoAbilityID::Cancel)
			)
		);

		ASCInputBound = true;
	}
}

void ADemoPlayerCharacter::InitializeStartingValues(ADemoPlayerState* PS)
{
	AbilitySystemComponent = Cast<UCharacterAbilitySystemComponent>(PS->GetAbilitySystemComponent());

	//we do this in playercontroller too,because ai may not have a controller
	//if ai don't controll this character,we don't really need this,but need to creat a aicharacter class
	PS->GetAbilitySystemComponent()->InitAbilityActorInfo(PS, this);

	AttributeSetBase = PS->GetAttributeSetBase();

	AbilitySystemComponent->SetTagMapCount(DeadTag, 0);

	InitializeAttributes();

	SetHealth(GetMaxHealth());
	SetMana(GetMaxMana());

}

//input (use enhancedinput
void ADemoPlayerCharacter::Look(const FInputActionValue& Value)
{
	if (IsAlive()) {

		FVector LookAxisVector = Value.Get<FVector>();

		if (Controller != nullptr) {
			AddControllerYawInput(LookAxisVector.X);
			AddControllerPitchInput(LookAxisVector.Y);
		}
	}

}

void ADemoPlayerCharacter::Move(const FInputActionValue& Value)
{
	if (IsAlive()) {
		FVector MovementVector = Value.Get<FVector>();

		if (Controller != nullptr) {
			//find out the way forward
			const FRotator Rotation = Controller->GetControlRotation();
			const FRotator YawRotation(0.f, Rotation.Yaw, 0.f);

			const FVector ForwardDirection = FRotationMatrix(YawRotation).GetUnitAxis(EAxis::X);
			const FVector RightDirection = FRotationMatrix(YawRotation).GetUnitAxis(EAxis::Y);

			AddMovementInput(ForwardDirection, MovementVector.Y);
			AddMovementInput(RightDirection, MovementVector.X);
		}
	}

}

void ADemoPlayerCharacter::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);
	if (PlayerInputComponent) {
		UEnhancedInputComponent* EnhancedInputComponent = Cast<UEnhancedInputComponent>(PlayerInputComponent);
		if (EnhancedInputComponent) {
			APlayerController* PlayerController = Cast<APlayerController>(Controller);
			UEnhancedInputLocalPlayerSubsystem* InputSubsystem = ULocalPlayer::GetSubsystem<UEnhancedInputLocalPlayerSubsystem>(PlayerController->GetLocalPlayer());
			if (InputSubsystem) {
				InputSubsystem->AddMappingContext(IMC_PlayerInput, 0);
			}

			EnhancedInputComponent->BindAction(IA_Move, ETriggerEvent::Triggered, this, &ADemoPlayerCharacter::Move);
			EnhancedInputComponent->BindAction(IA_Look, ETriggerEvent::Triggered, this, &ADemoPlayerCharacter::Look);
			EnhancedInputComponent->BindAction(IA_Jump, ETriggerEvent::Triggered, this, &ACharacter::Jump);
			EnhancedInputComponent->BindAction(IA_Jump, ETriggerEvent::Completed, this, &ACharacter::StopJumping);

			BindASCInput();

		}
	}
}

/*
*	for mouse
	void Turn(float Value);

	for gamepad
	void TurnRate(float Value);

	#include "Kismet/KismetMathLibrary.h"
	void MoveForward(float Value){
		AddMovement(UKismetMathLibrary::GetForwardVector(FRotator(0,GetControlRotation().Yaw,0)),value);
	}

	void MoveRight(float Value){
		AddMovement(UKismetMathLibrary::GetRightVector(FRotator(0,GetControlRotation().Yaw,0)),value);
	}


*/