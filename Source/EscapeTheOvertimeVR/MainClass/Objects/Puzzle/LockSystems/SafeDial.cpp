#include "MainClass/Objects/Puzzle/LockSystems/SafeDial.h"
#include "Components/StaticMeshComponent.h"
#include "GameFramework/PlayerController.h"

ASafeDial::ASafeDial()
{
    PrimaryActorTick.bCanEverTick = false;

    USceneComponent* Root = CreateDefaultSubobject<USceneComponent>(TEXT("DefaultRoot"));
    SetRootComponent(Root);

    SafeBodyMesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("SafeBodyMesh"));
    SafeBodyMesh->SetupAttachment(Root);

    SafeDoorMesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("SafeDoorMesh"));
    SafeDoorMesh->SetupAttachment(SafeBodyMesh);

    // --- 다이얼 컴포넌트 생성 및 물리 설정 강제화 ---
    auto InitializeDial = [this](UStaticMeshComponent*& Dial, FName Name) {
        Dial = CreateDefaultSubobject<UStaticMeshComponent>(Name);
        Dial->SetupAttachment(SafeDoorMesh);

        // [중요] 블루프린트에서 실수로 끄는 것을 방지하기 위해 코드로 강제 설정
        Dial->SetGenerateOverlapEvents(true);                    // 오버랩 이벤트 활성화
        Dial->SetCollisionProfileName(TEXT("OverlapAllDynamic")); // 손(HandSphere) 감지 가능 프로필
        Dial->SetCollisionEnabled(ECollisionEnabled::QueryOnly);  // 감지 전용 콜리전
        };

    InitializeDial(DialMesh_01, TEXT("Dial_01"));
    InitializeDial(DialMesh_02, TEXT("Dial_02"));
    InitializeDial(DialMesh_03, TEXT("Dial_03"));

    TargetAngles.Init(0.0f, 3);
    DialCorrectStates.Init(false, 3);
}

void ASafeDial::BeginPlay()
{
    Super::BeginPlay();

    // 런타임 관리를 위해 배열에 포인터 캐싱
    DialComponents.Empty();
    DialComponents.Add(DialMesh_01);
    DialComponents.Add(DialMesh_02);
    DialComponents.Add(DialMesh_03);

    // --- [추가] 매 로드 시 정답 각도 랜덤 설정 ---
    for (int32 i = 0; i < TargetAngles.Num(); i++)
    {
        // 0도 ~ 359도 사이의 정수형 랜덤 각도 부여 (정밀한 소수점은 맞추기 어려우므로)
        // 만약 특정 단위(예: 10도 단위)로 끊고 싶다면 (FMath::RandRange(0, 35) * 10.0f) 사용
        TargetAngles[i] = static_cast<float>(FMath::RandRange(0, 359));
    }
}

void ASafeDial::RotateDial(int32 DialIndex, float NewAngle, APlayerController* PC)
{
    if (bIsUnlocked || !DialComponents.IsValidIndex(DialIndex) || !DialComponents[DialIndex]) return;

    // 1. 각도 정규화 (0 ~ 360)
    float NormalizedAngle = FMath::Fmod(NewAngle, 360.0f);
    if (NormalizedAngle < 0) NormalizedAngle += 360.0f;

    // 2. 다이얼 회전 적용
    DialComponents[DialIndex]->SetRelativeRotation(FRotator(0, 0, NormalizedAngle));

    // 3. 정답과의 거리 계산
    float Diff = GetShortestAngleDistance(NormalizedAngle, TargetAngles[DialIndex]);

    // 4. 햅틱 피드백 (강화된 디버깅)
    if (PC && Diff <= MaxHapticDistance)
    {
        if (DialHapticEffect)
        {
            float Scale = 1.0f - (Diff / MaxHapticDistance);
            Scale = FMath::Clamp(Scale, 0.1f, 1.0f); // 최소 진동 보장

            // [팁] 컨트롤러 어느 쪽인지 확인 (현재는 우측 고정)
            PC->PlayHapticEffect(DialHapticEffect, EControllerHand::Right, Scale);

            // 만약 진동이 안 오면 이 로그가 출력되는지 확인하세요
            // UE_LOG(LogTemp, Warning, TEXT("Haptic Playing - Scale: %f"), Scale);
        }
    }

    DialCorrectStates[DialIndex] = (Diff <= Tolerance);
    CheckAllDials();
}


void ASafeDial::CheckAllDials()
{
    if (bIsUnlocked) return;

    for (bool bCorrect : DialCorrectStates)
    {
        if (!bCorrect) return; // 하나라도 틀리면 중단
    }

    // 모든 조건 충족 시 잠금 해제 이벤트 발생
    bIsUnlocked = true;
    OnSafeUnlocked();

    UE_LOG(LogTemp, Warning, TEXT("Safe Success: OnSafeUnlocked Triggered!"));
}

float ASafeDial::GetShortestAngleDistance(float A, float B)
{
    float Dist = FMath::Abs(A - B);
    if (Dist > 180.0f) Dist = 360.0f - Dist;
    return Dist;
}

