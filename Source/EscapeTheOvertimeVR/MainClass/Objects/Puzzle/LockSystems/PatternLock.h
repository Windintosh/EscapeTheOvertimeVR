// Copyright Epic Games, Inc. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "PatternLock.generated.h"

// 문 열림 이벤트를 위한 델리게이트 선언
DECLARE_DYNAMIC_MULTICAST_DELEGATE(FOnPatternUnlockedDelegate);

class UStaticMeshComponent;
class USoundBase;
class UMaterialInterface;

UCLASS()
class ESCAPETHEOVERTIMEVR_API APatternLock : public AActor
{
	GENERATED_BODY()

public:
	APatternLock();

protected:
	virtual void BeginPlay() override;

public:
	// --- 컴포넌트 (Components) ---

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Components")
	USceneComponent* RootScene;

	// 패드 본체
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Components")
	UStaticMeshComponent* BaseMesh;

	// 버튼 슬롯 1 (좌측)
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Components")
	UStaticMeshComponent* SlotMesh_01;

	// 버튼 슬롯 2 (중앙)
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Components")
	UStaticMeshComponent* SlotMesh_02;

	// 버튼 슬롯 3 (우측)
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Components")
	UStaticMeshComponent* SlotMesh_03;

	// 손 감지용 콜리전 (필요 시 사용)
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Components")
	class UBoxComponent* TouchCollision;


	// --- 설정 변수 (Config) ---

	// 교체될 문양 머티리얼 배열 (5가지: 동그라미, 세모, 네모, 다이아, 달)
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Puzzle Config")
	TArray<UMaterialInterface*> SymbolMaterials;

	// 정답 코드 (BeginPlay에서 랜덤 생성됨)
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Puzzle Config")
	TArray<int32> TargetCode;

	// 성공 시 재생할 사운드
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Puzzle Config")
	USoundBase* SuccessSound;

	// 버튼 클릭 시 재생할 사운드
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Puzzle Config")
	USoundBase* ClickSound;

	// 문 열림 이벤트 디스패처 (BP에서 Assign하여 문 열기 로직 연결)
	UPROPERTY(BlueprintAssignable, Category = "Events")
	FOnPatternUnlockedDelegate OnUnlocked;


	// --- 내부 상태 (Internal State) ---

	// 현재 입력된 코드
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Puzzle State")
	TArray<int32> CurrentCode;

	// 이미 풀렸는지 확인하는 플래그
	bool bIsSolved;


	// --- 함수 (Functions) ---

	// 슬롯 버튼 입력 처리 (VR 핸드에서 호출)
	UFUNCTION(BlueprintCallable, Category = "Puzzle Logic")
	void InteractSlot(int32 SlotIndex);

	// 정답 랜덤 생성 (게임 시작 시 호출)
	UFUNCTION(BlueprintCallable, Category = "Puzzle Logic")
	void RandomizeTargetCode();

	// [중요] 힌트 액터(PatternHint)가 정답 정보를 읽어가기 위한 Getter 함수
	UFUNCTION(BlueprintPure, Category = "Puzzle Access")
	TArray<int32> GetTargetCode() const { return TargetCode; }

	// 힌트 액터가 같은 머티리얼을 쓰기 위한 Getter 함수
	UFUNCTION(BlueprintPure, Category = "Puzzle Access")
	TArray<UMaterialInterface*> GetSymbolMaterials() const { return SymbolMaterials; }


protected:
	// 슬롯의 비주얼(머티리얼) 업데이트
	UFUNCTION(BlueprintCallable, Category = "Puzzle Logic")
	void UpdateSlotVisual(int32 SlotIndex);

	// 정답 확인 로직
	UFUNCTION(BlueprintCallable, Category = "Puzzle Logic")
	void CheckAnswer();

	// 편의를 위해 슬롯 인덱스로 해당 메쉬 컴포넌트를 반환하는 헬퍼 함수
	UStaticMeshComponent* GetMeshBySlotIndex(int32 SlotIndex) const;
};