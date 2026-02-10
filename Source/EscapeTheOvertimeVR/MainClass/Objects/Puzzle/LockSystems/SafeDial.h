#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "SafeDial.generated.h"

class UStaticMeshComponent;
class USceneComponent;
class APlayerController;
class UHapticFeedbackEffect_Base; // 전방 선언 추가

/**
 * 금고 본체, 문, 3개의 다이얼 컴포넌트를 통합 관리하는 클래스
 */
UCLASS()
class ESCAPETHEOVERTIMEVR_API ASafeDial : public AActor
{
    GENERATED_BODY()

public:
    ASafeDial();

protected:
    virtual void BeginPlay() override;

public:
    // --- 컴포넌트 (Components) ---

    UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Safe|Components")
    USceneComponent* DefaultRoot;

    UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Safe|Components")
    UStaticMeshComponent* SafeBodyMesh;

    UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Safe|Components")
    UStaticMeshComponent* SafeDoorMesh;

    UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Safe|Components")
    UStaticMeshComponent* DialMesh_01;

    UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Safe|Components")
    UStaticMeshComponent* DialMesh_02;

    UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Safe|Components")
    UStaticMeshComponent* DialMesh_03;

    // --- 설정 (Settings) ---

    /** 각 다이얼의 정답 각도 리스트 (3개 필요) */
    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Safe|Settings")
    TArray<float> TargetAngles;

    /** 정답 판정 오차 범위 */
    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Safe|Settings")
    float Tolerance = 5.0f;

    /** 진동이 느껴지기 시작하는 최대 각도 거리 */
    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Safe|Settings")
    float MaxHapticDistance = 45.0f;

    /** [추가] 다이얼 조작 시 사용할 햅틱 에셋 (에디터에서 할당) */
    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Safe|Settings")
    UHapticFeedbackEffect_Base* DialHapticEffect;

    // --- 로직 (Logic) ---

    /** 블루프린트에서 호출하여 다이얼을 회전시키고 상태를 체크하는 함수 */
    UFUNCTION(BlueprintCallable, Category = "Safe|Functions")
    void RotateDial(int32 DialIndex, float NewAngle, APlayerController* PC);

    /** 금고 잠금이 해제되었을 때 발생하는 이벤트 (BP에서 애니메이션 등 구현) */
    UFUNCTION(BlueprintImplementableEvent, Category = "Safe|Events")
    void OnSafeUnlocked();

    /** 현재 잠금 해제 상태 확인용 */
    UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Safe|State")
    bool bIsUnlocked = false;

private:
    void CheckAllDials();
    float GetShortestAngleDistance(float A, float B);

    /** 정답 여부 실시간 저장용 리스트 */
    TArray<bool> DialCorrectStates;

    /** 인덱스 관리를 위한 내부 배열 (GC 보호를 위해 UPROPERTY 추가) */
    UPROPERTY()
    TArray<UStaticMeshComponent*> DialComponents;
};