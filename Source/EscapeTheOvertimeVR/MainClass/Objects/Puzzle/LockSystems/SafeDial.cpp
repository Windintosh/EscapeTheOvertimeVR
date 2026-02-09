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

    DialMesh_01 = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("DialMesh_01"));
    DialMesh_01->SetupAttachment(SafeDoorMesh);

    DialMesh_02 = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("DialMesh_02"));
    DialMesh_02->SetupAttachment(SafeDoorMesh);

    DialMesh_03 = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("DialMesh_03"));
    DialMesh_03->SetupAttachment(SafeDoorMesh);

    TargetAngles.Init(0.0f, 3);
    DialCorrectStates.Init(false, 3);
}

void ASafeDial::BeginPlay()
{
    Super::BeginPlay();

    DialComponents.Empty();
    DialComponents.Add(DialMesh_01);
    DialComponents.Add(DialMesh_02);
    DialComponents.Add(DialMesh_03);
}

void ASafeDial::RotateDial(int32 DialIndex, float NewAngle, APlayerController* PC)
{
    if (bIsUnlocked || !DialComponents.IsValidIndex(DialIndex)) return;

    float NormalizedAngle = FMath::Fmod(NewAngle, 360.0f);
    if (NormalizedAngle < 0) NormalizedAngle += 360.0f;

    // 다이얼 메쉬 회전 적용
    DialComponents[DialIndex]->SetRelativeRotation(FRotator(0, 0, NormalizedAngle));

    float Diff = GetShortestAngleDistance(NormalizedAngle, TargetAngles[DialIndex]);

    // 햅틱 피드백
    if (PC && Diff <= MaxHapticDistance)
    {
        float Intensity = 1.0f - (Diff / MaxHapticDistance);
        Intensity = FMath::Clamp(Intensity, 0.0f, 1.0f);
        PC->PlayHapticEffect(nullptr, EControllerHand::Right, Intensity);
        PC->PlayHapticEffect(nullptr, EControllerHand::Left, Intensity);
    }

    DialCorrectStates[DialIndex] = (Diff <= Tolerance);
    CheckAllDials();
}

void ASafeDial::CheckAllDials()
{
    if (bIsUnlocked) return;

    for (bool bCorrect : DialCorrectStates)
    {
        if (!bCorrect) return;
    }

    bIsUnlocked = true;
    OnSafeUnlocked();
}

float ASafeDial::GetShortestAngleDistance(float A, float B)
{
    float Dist = FMath::Abs(A - B);
    if (Dist > 180.0f) Dist = 360.0f - Dist;
    return Dist;
}