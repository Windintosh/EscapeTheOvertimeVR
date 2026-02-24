#include "MainClass/Objects/Puzzle/LockSystems/NumberPad.h"
#include "Components/TextRenderComponent.h"
#include "Components/StaticMeshComponent.h"
#include "GameFramework/PlayerController.h"
#include "Haptics/HapticFeedbackEffect_Base.h"

ANumberPad::ANumberPad()
{
    PrimaryActorTick.bCanEverTick = false;

    // 컴포넌트 초기화
    DefaultRoot = CreateDefaultSubobject<USceneComponent>(TEXT("DefaultRoot"));
    SetRootComponent(DefaultRoot);

    PadMesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("PadMesh"));
    PadMesh->SetupAttachment(DefaultRoot);

    // 기본 색상 설정 (초록, 하양, 노랑, 빨강)
    SlotColors.Empty();
    SlotColors.Add(FLinearColor::Green);
    SlotColors.Add(FLinearColor::White);
    SlotColors.Add(FLinearColor::Yellow);
    SlotColors.Add(FLinearColor::Red);

    // 변수 초기화
    bIsLocked = false;
    CurrentInput = "";
}

void ANumberPad::BeginPlay()
{
    Super::BeginPlay();

    // 시작 시 한 번 초기화
    ResetInput();
}

void ANumberPad::PressNumber(int32 Number)
{
    // [강력 디버깅] 버튼이 눌렸는지 Output Log에 출력
    UE_LOG(LogTemp, Warning, TEXT("[NumberPad] Button %d Pressed! (Current Input: %s)"), Number, *CurrentInput);

    if (bIsLocked)
    {
        UE_LOG(LogTemp, Error, TEXT("[NumberPad] Input Blocked: System is LOCKED."));
        return;
    }

    if (CurrentInput.Len() >= 4)
    {
        UE_LOG(LogTemp, Error, TEXT("[NumberPad] Input Blocked: Max digits (4) reached."));
        return;
    }

    // 숫자 추가
    CurrentInput.AppendInt(Number);
    UpdateDisplay();

    // 햅틱 피드백 (오른손 기준)
    if (APlayerController* PC = GetWorld()->GetFirstPlayerController())
    {
        if (ClickHapticEffect)
        {
            PC->PlayHapticEffect(ClickHapticEffect, EControllerHand::Right);
        }
    }
}

void ANumberPad::PressEnter()
{
    UE_LOG(LogTemp, Error, TEXT("[NumberPad] ENTER Pressed! Final Input: %s"), *CurrentInput);

    if (bIsLocked) return;

    if (CurrentInput == CorrectPassword)
    {
        UE_LOG(LogTemp, Warning, TEXT("[NumberPad] PASSWORD CORRECT!"));
        bIsLocked = true;

        // 양손 햅틱 피드백
        if (APlayerController* PC = GetWorld()->GetFirstPlayerController())
        {
            if (SuccessHapticEffect)
            {
                PC->PlayHapticEffect(SuccessHapticEffect, EControllerHand::Right);
                PC->PlayHapticEffect(SuccessHapticEffect, EControllerHand::Left);
            }
        }

        OnSuccess(); // BP 이벤트
        OnNumberPadSuccessDispatcher.Broadcast(); // Level BP 이벤트
    }
    else
    {
        UE_LOG(LogTemp, Error, TEXT("[NumberPad] PASSWORD WRONG! (Target: %s)"), *CorrectPassword);
        OnFailure(); // BP 이벤트 (깜빡임 로직)
    }
}

void ANumberPad::UpdateDisplay()
{
    // [수정] Fatal에서 Error로 변경하여 엔진 종료(Crash)를 방지합니다.
    if (DisplaySlots.Num() == 0)
    {
        // 렌더링 로그에는 남지만, 엔진이 꺼지지는 않습니다.
        UE_LOG(LogTemp, Error, TEXT("[NumberPad] DisplaySlots array is EMPTY! Check your Blueprint. Skipping update to avoid crash."));
        return; // 배열이 비었으므로 아래 루프를 타지 않고 함수를 빠져나갑니다.
    }

    // 기존 루프 로직 시작
    for (int32 i = 0; i < 4; i++)
    {
        if (!DisplaySlots.IsValidIndex(i) || !DisplaySlots[i]) continue;

        if (i < CurrentInput.Len())
        {
            FString CharStr = CurrentInput.Mid(i, 1);
            DisplaySlots[i]->SetText(FText::FromString(CharStr));

            if (SlotColors.IsValidIndex(i))
            {
                DisplaySlots[i]->SetTextRenderColor(SlotColors[i].ToFColor(true));
            }
        }
        else
        {
            DisplaySlots[i]->SetText(FText::FromString(TEXT(" ")));
        }
    }
}

void ANumberPad::ResetInput()
{
    UE_LOG(LogTemp, Log, TEXT("[NumberPad] Resetting Input..."));
    CurrentInput = "";
    UpdateDisplay();
}