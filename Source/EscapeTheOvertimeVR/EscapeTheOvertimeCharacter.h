// Copyright Epic Games, Inc. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Character.h"
#include "Logging/LogMacros.h"
#include "Components/PawnNoiseEmitterComponent.h"
#include "EscapeTheOvertimeCharacter.generated.h"

class UInputComponent;
class USkeletalMeshComponent;
class UCameraComponent;
class UInputAction;
struct FInputActionValue;
class UAnimMontage;

DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FDistractionItemChangedDelegate, int32, DistractionItemQuantity);

DECLARE_LOG_CATEGORY_EXTERN(LogTemplateCharacter, Log, All);

USTRUCT(BlueprintType)
struct FItemState
{
	GENERATED_BODY()

	// 블루프린트에서 읽고 쓸 수 있게 허용
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Item Data")
	float Duration = 0.f;

	// 블루프린트에서 읽을 수 있게 허용
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Item Data")
	float RemainingTime = 0.f;
};



/**
 *  A basic first person character
 */
UCLASS(abstract)
class AEscapeTheOvertimeCharacter : public ACharacter
{
	GENERATED_BODY()

	/** Pawn mesh: first person view (arms; seen only by self) */
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category="Components", meta = (AllowPrivateAccess = "true"))
	USkeletalMeshComponent* FirstPersonMesh;

	/** First person camera */
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category="Components", meta = (AllowPrivateAccess = "true"))
	UCameraComponent* FirstPersonCameraComponent;

protected:

	/** Jump Input Action */
	UPROPERTY(EditAnywhere, Category ="Input")
	UInputAction* JumpAction;

	/** Move Input Action */
	UPROPERTY(EditAnywhere, Category ="Input")
	UInputAction* MoveAction;

	/** Look Input Action */
	UPROPERTY(EditAnywhere, Category ="Input")
	class UInputAction* LookAction;

	/** Mouse Look Input Action */
	UPROPERTY(EditAnywhere, Category ="Input")
	class UInputAction* MouseLookAction;

	UPROPERTY(EditAnywhere, Category = "Input")
	UInputAction* ThrowAction;

	UPROPERTY(EditAnywhere, Category = "Input")
	UInputAction* ShootAction;


	
public:
	AEscapeTheOvertimeCharacter();

protected:

	/** Called from Input Actions for movement input */
	void MoveInput(const FInputActionValue& Value);

	/** Called from Input Actions for looking input */
	void LookInput(const FInputActionValue& Value);

	/** Handles aim inputs from either controls or UI interfaces */
	UFUNCTION(BlueprintCallable, Category="Input")
	virtual void DoAim(float Yaw, float Pitch);

	/** Handles move inputs from either controls or UI interfaces */
	UFUNCTION(BlueprintCallable, Category="Input")
	virtual void DoMove(float Right, float Forward);

	/** Handles jump start inputs from either controls or UI interfaces */
	UFUNCTION(BlueprintCallable, Category="Input")
	virtual void DoJumpStart();

	/** Handles jump end inputs from either controls or UI interfaces */
	UFUNCTION(BlueprintCallable, Category="Input")
	virtual void DoJumpEnd();

	UFUNCTION(BlueprintCallable, Category = "Input")
	virtual void DoThrow();

	UFUNCTION(BlueprintCallable, Category = "Input")
	virtual void DoShoot();

protected:

	/** Set up input action bindings */
	virtual void SetupPlayerInputComponent(UInputComponent* InputComponent) override;
	
	virtual void Tick(float DeltaTime) override;

public:

	/** Returns the first person mesh **/
	USkeletalMeshComponent* GetFirstPersonMesh() const { return FirstPersonMesh; }

	/** Returns first person camera component **/
	UCameraComponent* GetFirstPersonCameraComponent() const { return FirstPersonCameraComponent; }

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Noise")
	class UPawnNoiseEmitterComponent* NoiseEmitterComponent;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Game")
	bool bHasKeycard = false;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Game")
	bool bIsMuffled = false;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Game")
	bool bIsSpedUp = false;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Item")
	int32 DistractionItemQuantity;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Item")
	int32 TQAmmoQuantity;

	UPROPERTY(BlueprintAssignable, Category = "Item")
	FDistractionItemChangedDelegate OnDistractionItemChanged;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Item")
	FItemState DistractionItem;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Item")
	FItemState MuffleItem;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Item")
	FItemState SpeedUpItem;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Item")
	TSubclassOf<AActor> ProjectileClass;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Item")
	float IncreasedSpeed = 1200.f;

	UFUNCTION(BlueprintCallable, Category = "Item")
	void ActivateMuffle();

	UFUNCTION(BlueprintCallable, Category = "Item")
	virtual void ActivateSpeedUp();

	UFUNCTION(BlueprintCallable, Category = "Item")
	void ActivateDistraction();

	UFUNCTION(BlueprintCallable, Category = "Item")
	void EndMuffle();

	UFUNCTION(BlueprintCallable, Category = "Item")
	void EndSpeedUp();

	UFUNCTION(BlueprintCallable, Category = "Item")
	void ActivateTQAmmo();

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Item")
	UAnimMontage* ThrowMontage;

	void MakeNoise(float Loudness, FVector NoiseLocation);

	// 마우스 감도 변수 (기본값 1.0)
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Input")
	float MouseSensitivity = 0.5f;

	// UI에서 호출할 감도 설정 함수
	UFUNCTION(BlueprintCallable, Category = "Input")
	void SetMouseSensitivity(float NewSensitivity);

private:
	
	bool bIsThrowing;

	UFUNCTION()
	void OnThrowEnded(UAnimMontage* Montage, bool bInterrupted);


};

