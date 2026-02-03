// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "MainClass/Objects/Doors/DoorBase.h"
#include "GimmickDoor.generated.h"

class USoundBase;

/**
 * 기본 도어(DoorBase)에 잠금 기능을 추가한 클래스입니다.
 * bIsLocked가 True일 때 Grab을 시도하면 문이 열리지 않고 잠긴 소리가 재생됩니다.
 */
UCLASS()
class ESCAPETHEOVERTIMEVR_API AGimmickDoor : public ADoorBase
{
	GENERATED_BODY()

public:
	AGimmickDoor();

protected:
	virtual void BeginPlay() override;

public:
	// --- 설정 변수 ---

	// 시작 시 잠금 여부 (기본값: True)
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Gimmick Door")
	bool bIsLocked;

	// 잠겨있을 때 상호작용 시 재생할 사운드 (철컥)
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Gimmick Door")
	USoundBase* LockedSound;

	// 잠금 해제 시 재생할 사운드 (달칵)
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Gimmick Door")
	USoundBase* UnlockSound;

	// --- 함수 ---

	// 부모 클래스(DoorBase)의 인터페이스 오버라이드
	// 잠겨있으면 문 열기 로직을 차단합니다.
	virtual void Grab_Implementation(USceneComponent* HandController) override;

	// 외부(퍼즐 등)에서 호출하여 잠금을 해제하는 함수
	// bOpenImmediately: 해제와 동시에 문을 열 것인지 설정
	UFUNCTION(BlueprintCallable, Category = "Gimmick Door")
	void UnlockDoor(bool bOpenImmediately = true);

	// 문을 다시 잠그는 함수
	UFUNCTION(BlueprintCallable, Category = "Gimmick Door")
	void LockDoor();
};