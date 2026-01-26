// Copyright Epic Games, Inc. All Rights Reserved.

#include "EscapeTheOvertimeCharacter.h"
#include "Animation/AnimInstance.h"
#include "Camera/CameraComponent.h"
#include "Components/CapsuleComponent.h"
#include "Components/SkeletalMeshComponent.h"
#include "EnhancedInputComponent.h"
#include "InputActionValue.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "HorrorCharacter.h"
#include "EscapeTheOvertimeVR.h"

AEscapeTheOvertimeCharacter::AEscapeTheOvertimeCharacter()
{
	// Set size for collision capsule
	GetCapsuleComponent()->InitCapsuleSize(55.f, 96.0f);
	
	// Create the first person mesh that will be viewed only by this character's owner
	FirstPersonMesh = CreateDefaultSubobject<USkeletalMeshComponent>(TEXT("First Person Mesh"));

	FirstPersonMesh->SetupAttachment(GetMesh());
	FirstPersonMesh->SetOnlyOwnerSee(true);
	FirstPersonMesh->FirstPersonPrimitiveType = EFirstPersonPrimitiveType::FirstPerson;
	FirstPersonMesh->SetCollisionProfileName(FName("NoCollision"));

	// Create the Camera Component	
	FirstPersonCameraComponent = CreateDefaultSubobject<UCameraComponent>(TEXT("First Person Camera"));
	FirstPersonCameraComponent->SetupAttachment(FirstPersonMesh, FName("head"));
	FirstPersonCameraComponent->SetRelativeLocationAndRotation(FVector(-2.8f, 5.89f, 0.0f), FRotator(0.0f, 90.0f, -90.0f));
	FirstPersonCameraComponent->bUsePawnControlRotation = true;
	FirstPersonCameraComponent->bEnableFirstPersonFieldOfView = true;
	FirstPersonCameraComponent->bEnableFirstPersonScale = true;
	FirstPersonCameraComponent->FirstPersonFieldOfView = 70.0f;
	FirstPersonCameraComponent->FirstPersonScale = 0.6f;

	// configure the character comps
	GetMesh()->SetOwnerNoSee(true);
	GetMesh()->FirstPersonPrimitiveType = EFirstPersonPrimitiveType::WorldSpaceRepresentation;

	GetCapsuleComponent()->SetCapsuleSize(34.0f, 96.0f);

	// Configure character movement
	GetCharacterMovement()->BrakingDecelerationFalling = 1500.0f;
	GetCharacterMovement()->AirControl = 0.5f;

	NoiseEmitterComponent = CreateDefaultSubobject<UPawnNoiseEmitterComponent>(TEXT("NoiseEmitter"));

	DistractionItem.Duration = 10.0f;
	MuffleItem.Duration = 30.f;
	SpeedUpItem.Duration = 10.f;


}

void AEscapeTheOvertimeCharacter::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{	
	// Set up action bindings
	if (UEnhancedInputComponent* EnhancedInputComponent = Cast<UEnhancedInputComponent>(PlayerInputComponent))
	{
		// Jumping
		EnhancedInputComponent->BindAction(JumpAction, ETriggerEvent::Started, this, &AEscapeTheOvertimeCharacter::DoJumpStart);
		EnhancedInputComponent->BindAction(JumpAction, ETriggerEvent::Completed, this, &AEscapeTheOvertimeCharacter::DoJumpEnd);

		// Moving
		EnhancedInputComponent->BindAction(MoveAction, ETriggerEvent::Triggered, this, &AEscapeTheOvertimeCharacter::MoveInput);

		// Looking/Aiming
		EnhancedInputComponent->BindAction(LookAction, ETriggerEvent::Triggered, this, &AEscapeTheOvertimeCharacter::LookInput);
		EnhancedInputComponent->BindAction(MouseLookAction, ETriggerEvent::Triggered, this, &AEscapeTheOvertimeCharacter::LookInput);

		EnhancedInputComponent->BindAction(ThrowAction, ETriggerEvent::Started, this, &AEscapeTheOvertimeCharacter::DoThrow);
	}
	else
	{
		UE_LOG(LogEscapeTheOvertimeVR, Error, TEXT("'%s' Failed to find an Enhanced Input Component! This template is built to use the Enhanced Input system. If you intend to use the legacy system, then you will need to update this C++ file."), *GetNameSafe(this));
	}
}

void AEscapeTheOvertimeCharacter::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	//if (bIsMuffled)
	//{
	//	MuffleItem.RemainingTime -= DeltaTime;
	//	UE_LOG(LogTemp, Warning, TEXT("Muffle : %f"), MuffleItem.RemainingTime);
	//	if (MuffleItem.RemainingTime <= 0)
	//	{
	//		EndMuffle();
	//	}
	//}

	if (bIsSpedUp)
	{
		SpeedUpItem.RemainingTime -= DeltaTime;
		UE_LOG(LogTemp, Warning, TEXT("SpeedUp : %f"), SpeedUpItem.RemainingTime);
		if (SpeedUpItem.RemainingTime <= 0)
		{
			EndSpeedUp();
		}
	}

	//UE_LOG(LogTemp, Warning, TEXT("Player Speed : %f"), GetCharacterMovement()->MaxWalkSpeed);
}

void AEscapeTheOvertimeCharacter::ActivateMuffle()
{
	bIsMuffled = true;
	//MuffleItem.RemainingTime = MuffleItem.Duration;
	UE_LOG(LogTemp, Warning, TEXT("Player is Muffled"));
}

void AEscapeTheOvertimeCharacter::ActivateSpeedUp()
{
	bIsSpedUp = true;
	GetCharacterMovement()->MaxWalkSpeed = GetCharacterMovement()->MaxWalkSpeed * 2;
	SpeedUpItem.RemainingTime = SpeedUpItem.Duration;
	UE_LOG(LogTemp, Warning, TEXT("SpeedUp Activated for %f sec"), SpeedUpItem.Duration);
}

void AEscapeTheOvertimeCharacter::ActivateDistraction()
{
	DistractionItemQuantity++;
	UE_LOG(LogTemp, Warning, TEXT("Got DistractionItem, you have %d"), DistractionItemQuantity);
	OnDistractionItemChanged.Broadcast(DistractionItemQuantity);//broadcast
}

void AEscapeTheOvertimeCharacter::EndMuffle()
{
	bIsMuffled = false;
	UE_LOG(LogTemp, Warning, TEXT("Muffle Ended"));
}

void AEscapeTheOvertimeCharacter::EndSpeedUp()
{
	bIsSpedUp = false;
	GetCharacterMovement()->MaxWalkSpeed = 250.f; //need to change logic
	UE_LOG(LogTemp, Warning, TEXT("SpeedUp Ended"));
}

void AEscapeTheOvertimeCharacter::MakeNoise(float Loudness, FVector NoiseLocation)
{
	if (bIsMuffled) return;

	if (NoiseEmitterComponent)
	{
		NoiseEmitterComponent->MakeNoise(this, Loudness, NoiseLocation);
		UE_LOG(LogTemp, Warning, TEXT("Noise Created at %s with loudness %f"), *NoiseLocation.ToString(), Loudness);
	}
}

void AEscapeTheOvertimeCharacter::OnThrowEnded(UAnimMontage* Montage, bool bInterrupted)
{
	bIsThrowing = false;
}


void AEscapeTheOvertimeCharacter::MoveInput(const FInputActionValue& Value)
{
	// get the Vector2D move axis
	FVector2D MovementVector = Value.Get<FVector2D>();

	// pass the axis values to the move input
	DoMove(MovementVector.X, MovementVector.Y);

}

void AEscapeTheOvertimeCharacter::LookInput(const FInputActionValue& Value)
{
	// get the Vector2D look axis
	FVector2D LookAxisVector = Value.Get<FVector2D>();

	// pass the axis values to the aim input
	DoAim(LookAxisVector.X, LookAxisVector.Y);

}

void AEscapeTheOvertimeCharacter::DoAim(float Yaw, float Pitch)
{
	if (GetController())
	{
		// pass the rotation inputs
		// 입력값(Yaw, Pitch)에 MouseSensitivity를 곱해서 회전 속도 조절
		AddControllerYawInput(Yaw * MouseSensitivity);
		AddControllerPitchInput(Pitch * MouseSensitivity);
	}
}


void AEscapeTheOvertimeCharacter::DoMove(float Right, float Forward)
{
	if (GetController())
	{
		// pass the move inputs
		AddMovementInput(GetActorRightVector(), Right);
		AddMovementInput(GetActorForwardVector(), Forward);
	}
}

void AEscapeTheOvertimeCharacter::DoJumpStart()
{
	// pass Jump to the character
	Jump();
}

void AEscapeTheOvertimeCharacter::DoJumpEnd()
{
	// pass StopJumping to the character
	StopJumping();
}

void AEscapeTheOvertimeCharacter::SetMouseSensitivity(float NewSensitivity)
{
	MouseSensitivity = NewSensitivity;
	UE_LOG(LogTemp, Warning, TEXT("Mouse Sensitivity Updated: %f"), MouseSensitivity);
}

void AEscapeTheOvertimeCharacter::DoThrow()
{
	if (ThrowMontage)
	{
		UAnimInstance* AnimInstance = GetMesh()->GetAnimInstance();
		if (AnimInstance)
		{
			if (DistractionItemQuantity > 0 && !bIsThrowing)
			{
				AnimInstance->Montage_Play(ThrowMontage); //play throw montage
				//DistractionItemQuantity--; //this is done in AnimNotify
				bIsThrowing = true;
				FOnMontageEnded EndDelegate;
				EndDelegate.BindUObject(this, &AEscapeTheOvertimeCharacter::OnThrowEnded);
				AnimInstance->Montage_SetEndDelegate(EndDelegate, ThrowMontage);
				UE_LOG(LogTemp, Display, TEXT("DistractionItem: %d"), DistractionItemQuantity);
			}
			else if (DistractionItemQuantity <= 0)
			{
				UE_LOG(LogTemp, Warning, TEXT("You don't have any DistractionItem!"));
			}
			else if(bIsThrowing)
			{
				UE_LOG(LogTemp, Warning, TEXT("You can't throw mid-thowing!"));
			}
			else 
			{
				UE_LOG(LogTemp, Error, TEXT("Throwing cannot be done!!"));
			}
		}
	}
}
