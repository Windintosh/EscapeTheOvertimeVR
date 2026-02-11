#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "NumberPad.generated.h"

// 컴포넌트 전방 선언
class UStaticMeshComponent;
class UTextRenderComponent;
class UHapticFeedbackEffect_Base;

// 성공 시 레벨 블루프린트 등에 알리기 위한 디스패처
DECLARE_DYNAMIC_MULTICAST_DELEGATE(FOnNumberPadSuccessSignature);

UCLASS()
class ESCAPETHEOVERTIMEVR_API ANumberPad : public AActor
{
    GENERATED_BODY()

public:
    ANumberPad();

protected:
    virtual void BeginPlay() override;

public:
    // --- 컴포넌트 섹션 ---
    UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "NumberPad|Components")
    USceneComponent* DefaultRoot;

    UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "NumberPad|Components")
    UStaticMeshComponent* PadMesh;

    /** 블루프린트에서 텍스트 렌더러 4개를 이 배열에 순서대로 넣어줘야 합니다. */
    UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "NumberPad|Components")
    TArray<UTextRenderComponent*> DisplaySlots;

    // --- 설정 섹션 ---
    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "NumberPad|Settings")
    FString CorrectPassword = TEXT("4573");

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "NumberPad|Settings")
    TArray<FLinearColor> SlotColors;

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "NumberPad|Settings")
    UHapticFeedbackEffect_Base* SuccessHapticEffect;

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "NumberPad|Settings")
    UHapticFeedbackEffect_Base* ClickHapticEffect;

    // --- 주요 기능 ---
    /** 버튼 클릭 시 호출 (0~9) */
    UFUNCTION(BlueprintCallable, Category = "NumberPad|Functions")
    void PressNumber(int32 Number);

    /** 엔터 버튼 클릭 시 호출 */
    UFUNCTION(BlueprintCallable, Category = "NumberPad|Functions")
    void PressEnter();

    /** 모든 입력 내용 초기화 */
    UFUNCTION(BlueprintCallable, Category = "NumberPad|Functions")
    void ResetInput();

    // --- 이벤트 (블루프린트에서 구현) ---
    UPROPERTY(BlueprintAssignable, Category = "NumberPad|Events")
    FOnNumberPadSuccessSignature OnNumberPadSuccessDispatcher;

    UFUNCTION(BlueprintImplementableEvent, Category = "NumberPad|Events")
    void OnSuccess();

    UFUNCTION(BlueprintImplementableEvent, Category = "NumberPad|Events")
    void OnFailure();

private:
    FString CurrentInput;
    bool bIsLocked;

    /** 화면 업데이트 로직 */
    void UpdateDisplay();
};