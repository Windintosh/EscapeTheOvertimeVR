#pragma once

#include "CoreMinimal.h"
#include "HorrorCharacter.h"
#include "Camera/CameraComponent.h"
#include "MotionControllerComponent.h"
#include "Components/SkeletalMeshComponent.h"
#include "InputActionValue.h"
#include "EnhancedInputComponent.h"
#include "EnhancedInputSubsystems.h"
#include "VRGrabInterface.h"
#include "VRCharacterPawn.generated.h"

UCLASS()
class ESCAPETHEOVERTIMEVR_API AVRCharacterPawn : public AHorrorCharacter
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

	// [이동]
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "VR Input")
	class UInputAction* VRMoveAction;

	// [회전]
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "VR Input")
	class UInputAction* VRTurnAction;

	// 이동 속도 (cm/s)
	UPROPERTY(EditAnywhere, Category = "VR Movement")
	float MWalkSpeed = 300.0f;

	// 스냅 턴 각도 (한 번 틱 할 때마다 돌아가는 각도)
	UPROPERTY(EditAnywhere, Category = "VR Movement")
	float SnapTurnAngle = 15.0f;

	// 트리거 입력 액션
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "VR Input")
	class UInputAction* TriggerLeftAction;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "VR Input")
	class UInputAction* TriggerRightAction;

protected:

	// 필수 오버라이드 함수
	//virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;

	// 잡기 범위 (반지름 cm 단위)
	UPROPERTY(EditDefaultsOnly, Category = "VR Interaction")
	float GrabRadius = 20.0f;

	// 현재 잡고 있는 액터를 저장 (놓을 때 필요)
	UPROPERTY() // 가비지 컬렉션 방지 및 안전한 참조를 위해 UPROPERTY 필수
	AActor* HeldActorLeft = nullptr;

	UPROPERTY()
	AActor* HeldActorRight = nullptr;

	// 내부 헬퍼 함수: 특정 손으로 잡기/놓기 시도
	void TryGrabActor(USceneComponent* HandMesh, AActor*& OutHeldActor);
	void TryReleaseActor(AActor*& InHeldActor, USceneComponent* HandMesh);

	// 입력 콜백 함수
	void OnGrabLeft(const FInputActionValue& Value);
	void OnGrabRight(const FInputActionValue& Value);

	// 콜백 함수
	void Move(const FInputActionValue& Value);
	void Turn(const FInputActionValue& Value);

	// 스냅 턴 중복 입력을 막기 위한 플래그
	bool bCanTurn = true;
	void ResetTurn();

	// 콜백 함수
	void OnTriggerLeft(const FInputActionValue& Value);
	void OnTriggerRight(const FInputActionValue& Value);

	// [달리기] Left Grab 입력을 받아 부모 함수를 호출하는 래퍼
	void OnSprintStart(const FInputActionValue& Value);
	void OnSprintEnd(const FInputActionValue& Value);

};
