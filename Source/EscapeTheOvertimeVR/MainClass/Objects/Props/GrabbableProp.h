// Copyright Epic Games, Inc. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "GrabbableProp.generated.h"

class UStaticMeshComponent;
class UBoxComponent;

/**
 * VR 환경에서 물리적으로 잡고 던질 수 있는 액터입니다.
 * 안정적인 물리를 위해 BoxComponent를 루트로 사용합니다.
 */
UCLASS()
class ESCAPETHEOVERTIMEVR_API AGrabbableProp : public AActor
{
	GENERATED_BODY()

public:
	AGrabbableProp();

protected:
	virtual void BeginPlay() override;

public:
	// --- 컴포넌트 (Components) ---

	// 물리와 충돌을 담당할 박스 콜리전 (루트)
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Components")
	UBoxComponent* GrabCollision;

	// 눈에 보이는 메쉬 (물리 없음, 장식용)
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Components")
	UStaticMeshComponent* PropMesh;

	// --- 그랩 기능 함수 ---

	UFUNCTION(BlueprintCallable, Category = "VR Interaction")
	void Grab(USceneComponent* Hand);

	UFUNCTION(BlueprintCallable, Category = "VR Interaction")
	void Release();

	// --- 상태 변수 ---

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "VR Interaction")
	bool bIsHeld;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "VR Interaction")
	USceneComponent* HoldingHand;
};