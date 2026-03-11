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
#include "Components/PostProcessComponent.h"
#include "VRCharacterPawn.generated.h"


UENUM(BlueprintType)
enum class EItemSlot : uint8
{
	None        UMETA(DisplayName = "Empty Hand"), // 빈손 (일반 상호작용)
	Tranquilizer      UMETA(DisplayName = "Tranquilizer"), // 마취총
	ThrownItem         UMETA(DisplayName = "ThrownItem"),    // 머그컵
	Max         UMETA(Hidden) // 개수 파악용
};

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

	virtual float TakeDamage(float DamageAmount, struct FDamageEvent const& DamageEvent, class AController* EventInstigator, AActor* DamageCauser) override;

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

	// [인벤토리] 아이템 변경 버튼 (A/B 버튼 등)
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "VR Input")
	class UInputAction* NextItemAction;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "VR Input")
	class UInputAction* PrevItemAction;

	// [스폰] 소환할 아이템 클래스 (블루프린트에서 지정)
	UPROPERTY(EditAnywhere, Category = "Inventory")
	TSubclassOf<AActor> TranquilizerClass;

	UPROPERTY(EditAnywhere, Category = "Inventory")
	TSubclassOf<AActor> ThrownItemClass;
	
	void SpawnAndEquip(TSubclassOf<AActor> ClassToSpawn, USceneComponent* HandMesh, AActor*& HeldActorRef);

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Inventory")
	bool bHasTranquilizer = false;

	UFUNCTION(BlueprintCallable, Category = "Inventory")
	void GiveTranquilizer();

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Inventory")
	bool bGotGolden = false;

	UFUNCTION(BlueprintCallable, Category = "VR Interaction")
	void ForceDropItem(AActor* ItemToDrop);

	

protected:

	// 필수 오버라이드 함수
	//virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;

	// 잡기 범위 (반지름 cm 단위)
	UPROPERTY(EditDefaultsOnly, Category = "VR Interaction")
	float GrabRadius = 20.0f;

	// 현재 잡고 있는 액터를 저장 (놓을 때 필요)
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "VR Interaction") // 가비지 컬렉션 방지 및 안전한 참조를 위해 UPROPERTY 필수
	AActor* HeldActorLeft = nullptr;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "VR Interaction")
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

	// 현재 선택된 슬롯
	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = "Inventory")
	EItemSlot CurrentItemSlot = EItemSlot::None;

	// 슬롯 변경 함수
	void CycleItemNext(const FInputActionValue& Value);
	void CycleItemPrev(const FInputActionValue& Value);

	// 로그 출력용 헬퍼
	void PrintCurrentSlot();

	// 직접 계산한 오른손 속도
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "VR Physics")
	FVector CalculatedRightHandVelocity;

	// 이전 프레임의 오른손 위치
	FVector LastRightHandLocation;
	
	//Post process component for visual effects (e.g., when picking up certain items)
protected:
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "VR Effects")
	UPostProcessComponent* DamagePostProcessComp;

	UPROPERTY()
	class UMaterialInstanceDynamic* DamageMaterialInstance;

	UPROPERTY(EditDefaultsOnly, Category = "VR Effects")
	class UMaterialInterface* DamageVignetteMaterialBase;

	FTimerHandle DamageEffectTimerHandle;
	float CurrentDamageIntensity;

public:
	void PlayDamageEffect();

private:
	void UpdateDamageEffect();
	
};
