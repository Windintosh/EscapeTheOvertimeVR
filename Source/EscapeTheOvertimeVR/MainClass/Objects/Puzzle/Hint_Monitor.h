// Copyright Epic Games, Inc. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "Hint_Monitor.generated.h"

class UStaticMeshComponent;
class UBoxComponent;
class USoundBase;
class APatternLock;

/**
 * APatternLock과 연결되어 정답 힌트를 보여주는 모니터 액터입니다.
 * 본체를 건드리면(Toggle) 연결된 Lock의 정답 문양을 모니터 화면에 띄웁니다.
 */
UCLASS()
class ESCAPETHEOVERTIMEVR_API AHint_Monitor : public AActor
{
	GENERATED_BODY()

public:
	AHint_Monitor();

protected:
	virtual void BeginPlay() override;

public:
	// --- 컴포넌트 ---
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Components")
	USceneComponent* RootScene;

	// 힌트 본체 (컴퓨터 본체 등)
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Components")
	UStaticMeshComponent* BodyMesh;

	// 상호작용 트리거 (본체 주변 손 감지용)
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Components")
	UBoxComponent* InteractTrigger;

	// 힌트 모니터 1~3 (화면 표시용)
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Components")
	UStaticMeshComponent* MonitorMesh_01;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Components")
	UStaticMeshComponent* MonitorMesh_02;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Components")
	UStaticMeshComponent* MonitorMesh_03;

	// --- 설정 (Config) ---

	// [연결] 정답 정보를 가져올 패턴락 액터
	UPROPERTY(EditInstanceOnly, BlueprintReadWrite, Category = "Puzzle Config")
	APatternLock* TargetLock;

	// [중요] 모니터 메쉬에서 '화면'에 해당하는 머티리얼 인덱스 (보통 1번)
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Puzzle Config")
	int32 MonitorScreenMaterialIndex;

	// 모니터가 꺼져있을 때 보여줄 화면 머티리얼 (검은색/노이즈 등)
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Puzzle Config")
	UMaterialInterface* MonitorOffMaterial;

	// 전원 켜짐/꺼짐 사운드
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Puzzle Config")
	USoundBase* ToggleSound;

	// 현재 켜져있는지 상태
	bool bIsHintOn;

	// --- 함수 ---

	// 외부(VR 손)에서 호출할 함수: 전원 토글
	UFUNCTION(BlueprintCallable, Category = "Puzzle Logic")
	void ToggleHint();

	// 화면 비주얼 업데이트 (내부 로직)
	void UpdateMonitorVisual();
};