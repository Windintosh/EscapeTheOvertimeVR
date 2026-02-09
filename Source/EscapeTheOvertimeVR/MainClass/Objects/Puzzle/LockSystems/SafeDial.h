#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "SafeDial.generated.h"

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
    // --- 컴포넌트 ---
    UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Components")
    UStaticMeshComponent* SafeBodyMesh;

    UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Components")
    UStaticMeshComponent* SafeDoorMesh;

    UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Components")
    UStaticMeshComponent* DialMesh_01;

    UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Components")
    UStaticMeshComponent* DialMesh_02;

    UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Components")
    UStaticMeshComponent* DialMesh_03;

    // --- 설정 ---
    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Safe Settings")
    TArray<float> TargetAngles;

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Safe Settings")
    float Tolerance = 5.0f;

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Safe Settings")
    float MaxHapticDistance = 45.0f;

    // --- 로직 ---
    UFUNCTION(BlueprintCallable, Category = "Safe Logic")
    void RotateDial(int32 DialIndex, float NewAngle, APlayerController* PC);

    UFUNCTION(BlueprintImplementableEvent, Category = "Safe Events")
    void OnSafeUnlocked();

private:
    void CheckAllDials();
    float GetShortestAngleDistance(float A, float B);

    TArray<bool> DialCorrectStates;
    TArray<UStaticMeshComponent*> DialComponents;
    bool bIsUnlocked = false;
};