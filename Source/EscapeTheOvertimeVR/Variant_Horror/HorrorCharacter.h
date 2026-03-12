#pragma once

#include "CoreMinimal.h"
#include "InputActionValue.h"
#include "EscapeTheOvertimeCharacter.h"
#include "Components/AudioComponent.h"
#include "HorrorCharacter.generated.h"

class USpotLightComponent;
class UInputAction;
class USoundBase;
class UTexture2D;

// 사망 장소 구분을 위한 열거형 (Enum) 정의
UENUM(BlueprintType)
enum class EDeathLocationType : uint8
{
	//None        UMETA(DisplayName = "None"),	   //no use, removed
	Office      UMETA(DisplayName = "Office"),     // 사무실
	Window      UMETA(DisplayName = "Window"),     // 창가
	Pantry      UMETA(DisplayName = "Pantry"),     // 탕비실
	Hallway     UMETA(DisplayName = "Hallway"),    // 복도
	Restroom    UMETA(DisplayName = "Restroom"),    // 화장실
	TimeOver    UMETA(DisplayName = "TimeOver")
};

// 스태미나 관련 델리게이트
DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FUpdateSprintMeterDelegate, float, Percentage);
DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FSprintStateChangedDelegate, bool, bSprinting);

// HP 변경 알림용 델리게이트
DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FOnHealthChangedDelegate, float, HPPercent);

// 아이템 획득 알림용 델리게이트
DECLARE_DYNAMIC_MULTICAST_DELEGATE_ThreeParams(FOnItemPickedUpDelegate, FText, Name, FText, Desc, UTexture2D*, Icon);

/**
 * Simple first person horror character
 * Provides stamina-based sprinting and Health System
 */
UCLASS(abstract)
class ESCAPETHEOVERTIMEVR_API AHorrorCharacter : public AEscapeTheOvertimeCharacter
{
	GENERATED_BODY()

	/** Player light source */
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Components", meta = (AllowPrivateAccess = "true"))
	USpotLightComponent* SpotLight;

protected:

	/** Fire weapon input action */
	UPROPERTY(EditAnywhere, Category = "Input")
	UInputAction* SprintAction;

	/** If true, we're sprinting */
	bool bSprinting = false;

	/** If true, we're recovering stamina */
	bool bRecovering = false;

	//bool bIsSpedUp = false;

	//float IncreasedSpeed = 1200.0f; // 필요시 값 조정

	/** Default walk speed when not sprinting or recovering */
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Walk")
	float WalkSpeed = 250.0f;

	/** Time interval for sprinting stamina ticks */
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Sprint", meta = (ClampMin = 0, ClampMax = 1, Units = "s"))
	float SprintFixedTickTime = 0.03333f;

	/** Sprint stamina amount. Maxes at SprintTime */
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Sprint")
	float SprintMeter = 0.0f;

	/** How long we can sprint for, in seconds */
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Sprint", meta = (ClampMin = 0, ClampMax = 10, Units = "s"))
	float SprintTime = 5.0f;

	/** Walk speed while sprinting */
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Sprint", meta = (ClampMin = 0, Units = "cm/s"))
	float SprintSpeed = 600.0f;

	/** Walk speed while recovering stamina */
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Recovery", meta = (ClampMin = 0, Units = "cm/s"))
	float RecoveringWalkSpeed = 150.0f;

	/** Time it takes for the sprint meter to recover */
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Recovery", meta = (ClampMin = 0, ClampMax = 10, Units = "s"))
	float RecoveryTime = 0.0f;

	/** Sprint tick timer */
	FTimerHandle SprintTimer;

	/* ================================================== */
	/* HP & Death System                          */
	/* ================================================== */

	/** Max HP */
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Health")
	float MaxHP = 100.0f;

	/** Current HP */
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Health")
	float CurrentHP;

	/** Is Dead? */
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Health")
	bool bIsDead = false;

	/** 피격 시 재생할 사운드 (Sound Cue) */
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Audio")
	USoundBase* DamageSound;

	//  현재 플레이어가 위치한 장소 (기본값 None) 트리거 박스에 들어갈 때마다 이 변수가 갱신
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Cinematic")
	EDeathLocationType CurrentDeathLocation;

public:

	/** Delegate called when the sprint meter should be updated */
	FUpdateSprintMeterDelegate OnSprintMeterUpdated;

	/** Delegate called when we start and stop sprinting */
	FSprintStateChangedDelegate OnSprintStateChanged;

	// 맞았을 때만 UI를 업데이트하기 위한 이벤트
	UPROPERTY(BlueprintAssignable, Category = "Health")
	FOnHealthChangedDelegate OnHealthChanged;

	// 아이템 획득 시 방송(Broadcast)할 이벤트 디스패처
	UPROPERTY(BlueprintAssignable, Category = "UI")
	FOnItemPickedUpDelegate OnItemPickedUp;

	// 외부에서 HP 정보를 가져오기 위한 유틸리티 함수
	UFUNCTION(BlueprintPure, Category = "Health")
	float GetCurrentHP() const { return CurrentHP; }

	UFUNCTION(BlueprintPure, Category = "Health")
	float GetMaxHP() const { return MaxHP; }

	UFUNCTION(BlueprintPure, Category = "Health")
	bool IsDead() const { return bIsDead; }

	UFUNCTION(Category = "Health")
	void Heal(float Amount);

	UFUNCTION(Category = "Health")
	void AddMaxHP(float Amount);

	UFUNCTION(BlueprintCallable, Category = "Health")
	void SetHP(float Value);

	// 사망 시 실행할 이벤트
	UFUNCTION(BlueprintNativeEvent, Category = "Health")
	void OnDeath();

	//UFUNCTION(Category = "Health")
	//void DamagePlayer(float Amount);

	// 외부(트리거 박스 등)에서 현재 플레이어의 장소 정보를 갱신할 때 호출하는 함수
	UFUNCTION(BlueprintCallable, Category = "Cinematic")
	void SetDeathLocation(EDeathLocationType NewLocation);

	// 앉기 입력 액션 (에디터에서 할당)
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Input")
	class UInputAction* CrouchAction;

	//virtual void ActivateSpeedUp() override;

	//UFUNCTION(BlueprintCallable, Category = "Gameplay")
	//void SetSpeedUpState(bool bNewState);

	bool GetSprintState() const;

	// 심장박동 사운드 컴포넌트
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Audio")
	UAudioComponent* HeartbeatAudioComponent;

	// AI 감지 상태 업데이트 함수 (True: 발각됨, False: 따돌림)
	UFUNCTION(BlueprintCallable, Category = "AI")
	void UpdateHeartbeatState(bool bIsDetected);

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Essentials")
	bool bIsGameCleared = false;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Essentials")
	bool bGotPill = false;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Essentials")
	bool bGotIDCard = false;

protected:

	/** Constructor */
	AHorrorCharacter();

	/** Gameplay initialization */
	virtual void BeginPlay() override;

	/** Gameplay cleanup */
	virtual void EndPlay(EEndPlayReason::Type EndPlayReason) override;

	/** Set up input action bindings */
	virtual void SetupPlayerInputComponent(UInputComponent* InputComponent) override;

	// 언리얼 엔진 기본 피격 함수 오버라이드
	virtual float TakeDamage(float DamageAmount, struct FDamageEvent const& DamageEvent, class AController* EventInstigator, AActor* DamageCauser) override;

	virtual void OnDeath_Implementation();

	//  하이브리드 방식: C++에서 호출하면 블루프린트에서 시네마틱을 재생하는 이벤트
	UFUNCTION(BlueprintImplementableEvent, Category = "Cinematic")
	void PlayGameOverCinematic(EDeathLocationType DeathLocation);

	// 앉기 시작/종료 함수
	void StartCrouch(const FInputActionValue& Value);
	void StopCrouch(const FInputActionValue& Value);

protected:

	/** Starts sprinting behavior */
	UFUNCTION(BlueprintCallable, Category = "Input")
	void DoStartSprint();

	/** Stops sprinting behavior */
	UFUNCTION(BlueprintCallable, Category = "Input")
	void DoEndSprint();

	/** Called while sprinting at a fixed time interval */
	void SprintFixedTick();

	// 나를 쫓고 있는 적의 수 (0명이 되면 소리 멈춤)
	int32 ChasingEnemyCount = 0;

	UFUNCTION(BlueprintCallable, Category = "Input")
	void StunPlayer();

	FTimerHandle StunTimerHandle;

	UFUNCTION(BlueprintCallable, Category = "Input")
	void RecoverFromStun();
};
