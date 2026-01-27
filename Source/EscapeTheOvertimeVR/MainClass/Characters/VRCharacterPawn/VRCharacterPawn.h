#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Pawn.h"
#include "Camera/CameraComponent.h"
#include "MotionControllerComponent.h"
#include "Components/SkeletalMeshComponent.h"
#include "InputActionValue.h"
#include "EnhancedInputComponent.h"
#include "EnhancedInputSubsystems.h"
#include "VRCharacterPawn.generated.h"

UCLASS()
class ESCAPETHEOVERTIMEVR_API AVRCharacterPawn : public APawn
{
	GENERATED_BODY()

public:
	// Sets default values for this pawn's properties
	AVRCharacterPawn();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	// Called to bind functionality to input
	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;

	// 1. VROrigin: VR 공간의 기준점 (바닥)
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "VR")
	USceneComponent* VROrigin;

	// 2. Camera: HMD(헤드셋)의 위치와 회전을 추적
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "VR")
	UCameraComponent* VRCamera;

	// 3. Motion Controllers: 양손 컨트롤러
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "VR")
	UMotionControllerComponent* LeftHandController;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "VR")
	UMotionControllerComponent* RightHandController;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "VR")
	USkeletalMeshComponent* LeftHandMesh;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "VR")
	USkeletalMeshComponent* RightHandMesh;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Input")
	class UInputMappingContext* DefaultMappingContext;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Input")
	class UInputAction* GrabLeftAction;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Input")
	class UInputAction* GrabRightAction;

protected:

	// 필수 오버라이드 함수
	//virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;

	// 입력 콜백 함수
	void OnGrabLeft(const FInputActionValue& Value);
	void OnGrabRight(const FInputActionValue& Value);
};
