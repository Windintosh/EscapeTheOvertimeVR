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
 * VR 환경 및 10개 이상의 시네마틱 분기 시스템을 관리하는 컨트롤러
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

	virtual void StartCinematic_Implementation(ALevelSequenceActor* SequenceActor, ACineCameraActor* CameraActor) override;
	virtual void EndCinematic_Implementation() override;

	/** 시퀀서 종료 시 호출될 내부 래퍼 함수 */
	UFUNCTION()
	void Internal_OnCinematicFinished();

public:
	/* ===============================
	   🕒 Time System (Public for ETOGameState Access)
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

	bool bIsGameOver = false;

	UFUNCTION(BlueprintCallable)
	void InitCurrentTime();

	/** 시간 종료 시 BP에서 호출될 이벤트 */
	UFUNCTION(BlueprintImplementableEvent, Category = "Time System")
	void OnTimeLimitReached();

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
	UPROPERTY()
	APawn* OriginalPawn = nullptr;

	UPROPERTY()
	APawn* CinematicPawnInstance = nullptr;

	/** 시네마틱 재생용 VR 빈 폰 (BP에서 할당 필수) */
	UPROPERTY(EditDefaultsOnly, Category = "Cinematic")
	TSubclassOf<APawn> CinematicPawnClass;
};