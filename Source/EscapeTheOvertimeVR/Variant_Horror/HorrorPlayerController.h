// Copyright Epic Games, Inc. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/PlayerController.h"
#include "MainClass/Game/CinematicControlInterface.h"
#include "HorrorPlayerController.generated.h"

class UInputMappingContext;
class UHorrorUI;
class ALevelSequenceActor;
class ACineCameraActor;
class UUserWidget;

/**
 * VR 환경 및 시네마틱 분기 시스템, 인게임 시간/UI를 관리하는 메인 컨트롤러
 */
UCLASS(Blueprintable)
class ESCAPETHEOVERTIMEVR_API AHorrorPlayerController : public APlayerController, public ICinematicControlInterface
{
	GENERATED_BODY()

public:
	AHorrorPlayerController();

	virtual void BeginPlay() override;
	virtual void Tick(float DeltaTime) override;
	virtual void OnPossess(APawn* aPawn) override;
	virtual void SetupInputComponent() override;

	/* ===============================
	   🎬 Cinematic System (Interface)
	   =============================== */

	   /** * 파라미터가 제거된 StartCinematic 구현부입니다.
		*/
	virtual void StartCinematic_Implementation(ALevelSequenceActor* SequenceActor, ACineCameraActor* CameraActor) override;

	virtual void EndCinematic_Implementation() override;

	/** 시퀀서 종료 시 호출될 내부 래퍼 함수 */
	UFUNCTION()
	void Internal_OnCinematicFinished();

	/** 현재 재생 중인 시네마틱을 끝으로 이동시키고 종료(스킵)합니다. */
	UFUNCTION(BlueprintCallable, Category = "Cinematic")
	void SkipCurrentCinematic();

public:
	/* ===============================
	   🕒 Time System
	   =============================== */

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Time System")
	int32 CurrentHour;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Time System")
	float CurrentMinute;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Time System")
	float ElapsedTime = 0.0f;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Time System")
	float TimeSpeed = 4.0f;

	UPROPERTY(BlueprintReadWrite, Category = "Time System")
	bool bIsPaused = false;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Time System")
	bool bIsGameOver = false;

	UFUNCTION(BlueprintCallable)
	void InitCurrentTime();

	/** 시간 종료 시 BP에서 호출될 이벤트 */
	UFUNCTION(BlueprintImplementableEvent, Category = "Time System")
	void OnTimeLimitReached();

	/** HorrorCharacter.cpp의 에러 해결을 위한 Getter 함수 */
	UFUNCTION(BlueprintPure, Category = "Time System")
	bool GetTimeLimitReached() const { return bIsTimeLimitReached; }

	/* ===============================
	   🎮 UI / Input / Mobile
	   =============================== */

	UPROPERTY(EditAnywhere, Category = "Horror|UI")
	TSubclassOf<UHorrorUI> HorrorUIClass;

	UPROPERTY()
	UHorrorUI* HorrorUI;

	UPROPERTY(EditAnywhere, Category = "Input")
	TArray<UInputMappingContext*> DefaultMappingContexts;

	UPROPERTY(EditAnywhere, Category = "Input")
	TArray<UInputMappingContext*> MobileExcludedMappingContexts;

	UPROPERTY(EditAnywhere, Category = "Input|Mobile")
	TSubclassOf<UUserWidget> MobileControlsWidgetClass;

	UPROPERTY()
	UUserWidget* MobileControlsWidget;

protected:
	/** 원래 조작하던 Pawn 저장용 */
	UPROPERTY()
	APawn* OriginalPawn = nullptr;

	/** 현재 재생 중인 시퀀스 액터 저장용 (스킵 시 사용) */
	UPROPERTY(BlueprintReadOnly, Category = "Cinematic")
	ALevelSequenceActor* ActiveSequenceActor = nullptr;

	/** 시네마틱 재생 시 생성되는 임시 시점용 폰 */
	UPROPERTY()
	APawn* CinematicPawnInstance = nullptr;

	/** 시네마틱 재생용 VR 빈 폰 (BP에서 할당 필수) */
	UPROPERTY(EditDefaultsOnly, Category = "Cinematic")
	TSubclassOf<APawn> CinematicPawnClass;

	/** 시간 제한 도달 여부를 저장하는 내부 변수 */
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Time System")
	bool bIsTimeLimitReached = false;
};