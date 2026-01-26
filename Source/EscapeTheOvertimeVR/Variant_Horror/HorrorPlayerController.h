// Copyright Epic Games, Inc. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/PlayerController.h"
#include "HorrorPlayerController.generated.h"

class UInputMappingContext;
class UHorrorUI;

/**
 * Player Controller for a first person horror game
 * Manages input mappings
 * Manages UI
 * Manages Time System (Added)
 */
UCLASS(abstract)
class ESCAPETHEOVERTIMEVR_API AHorrorPlayerController : public APlayerController
{
	GENERATED_BODY()

protected:

	/** Type of UI widget to spawn */
	UPROPERTY(EditAnywhere, Category = "Horror|UI")
	TSubclassOf<UHorrorUI> HorrorUIClass;

	/** Pointer to the UI widget */
	TObjectPtr<UHorrorUI> HorrorUI;

public:

	/** Constructor */
	AHorrorPlayerController();

	// [Time System] 
	virtual void Tick(float DeltaTime) override;

protected:

	/** Input Mapping Contexts */
	UPROPERTY(EditAnywhere, Category = "Input|Input Mappings")
	TArray<UInputMappingContext*> DefaultMappingContexts;

	/** Input Mapping Contexts */
	UPROPERTY(EditAnywhere, Category = "Input|Input Mappings")
	TArray<UInputMappingContext*> MobileExcludedMappingContexts;

	/** Mobile controls widget to spawn */
	UPROPERTY(EditAnywhere, Category = "Input|Touch Controls")
	TSubclassOf<UUserWidget> MobileControlsWidgetClass;

	/** Pointer to the mobile controls widget */
	TObjectPtr<UUserWidget> MobileControlsWidget;

	/** Gameplay Initialization */
	virtual void BeginPlay() override;

	/** Possessed pawn initialization */
	virtual void OnPossess(APawn* aPawn) override;

	/** Input mapping context setup */
	virtual void SetupInputComponent() override;

public:
	// --- [Time System Variables] ---

	/** 현재 시간 (기본값 18시) */
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Time System")
	int32 CurrentHour;

	/** 현재 분 (누적용 Float) */
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Time System")
	float CurrentMinute;

	/** 시간 흐름 속도 (1.0 = 실제 1초가 게임 1분) */
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Time System")
	float TimeSpeed;

	/** 게임 오버 상태 체크 */
	bool bIsGameOver;

	UFUNCTION(BlueprintCallable)
	void InitCurrentTime();

	UPROPERTY(BlueprintReadWrite)
	bool bIsPaused = false;

protected:
	// --- [Time System Events] ---

	/** 12시(자정)가 되었을 때 블루프린트에서 호출될 이벤트 (암전, 게임오버 연출용) */
	UFUNCTION(BlueprintImplementableEvent, Category = "Time System")
	void OnTimeLimitReached();

};
