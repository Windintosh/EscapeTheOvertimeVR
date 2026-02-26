// Copyright Epic Games, Inc. All Rights Reserved.

#include "Variant_Horror/HorrorPlayerController.h"
#include "EnhancedInputSubsystems.h"
#include "LevelSequenceActor.h"
#include "LevelSequencePlayer.h"
#include "CineCameraActor.h"
#include "HeadMountedDisplayFunctionLibrary.h"
#include "Blueprint/UserWidget.h"
#include "HorrorCharacter.h"
#include "HorrorUI.h"
#include "Kismet/GameplayStatics.h"

AHorrorPlayerController::AHorrorPlayerController()
{
	PrimaryActorTick.bCanEverTick = true;
	CurrentHour = 20;
	CurrentMinute = 0.0f;
	TimeSpeed = 4.0f;
}

void AHorrorPlayerController::BeginPlay()
{
	Super::BeginPlay();
	OriginalPawn = GetPawn();
	UHeadMountedDisplayFunctionLibrary::SetTrackingOrigin(EHMDTrackingOrigin::Stage);

	if (IsLocalPlayerController() && MobileControlsWidgetClass)
	{
		MobileControlsWidget = CreateWidget<UUserWidget>(this, MobileControlsWidgetClass);
		if (MobileControlsWidget) MobileControlsWidget->AddToPlayerScreen(0);
	}
}

void AHorrorPlayerController::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
	if (bIsGameOver || bIsPaused) return;

	CurrentMinute += DeltaTime * TimeSpeed;
	ElapsedTime += DeltaTime;

	if (CurrentMinute >= 60.0f)
	{
		CurrentMinute -= 60.0f;
		CurrentHour++;
		if (CurrentHour >= 24)
		{
			bIsGameOver = true;
			if (AHorrorCharacter* MyChar = Cast<AHorrorCharacter>(GetPawn())) MyChar->OnDeath();
			OnTimeLimitReached();
		}
	}
}

void AHorrorPlayerController::OnPossess(APawn* aPawn)
{
	Super::OnPossess(aPawn);
	if (IsLocalPlayerController() && aPawn && HorrorUIClass)
	{
		if (!HorrorUI)
		{
			HorrorUI = CreateWidget<UHorrorUI>(this, HorrorUIClass);
			if (HorrorUI) HorrorUI->AddToViewport(0);
		}
		if (AHorrorCharacter* HorrorCharacter = Cast<AHorrorCharacter>(aPawn))
		{
			if (HorrorUI) HorrorUI->SetupCharacter(HorrorCharacter);
		}
	}
}

void AHorrorPlayerController::SetupInputComponent()
{
	Super::SetupInputComponent();
	if (IsLocalPlayerController())
	{
		if (UEnhancedInputLocalPlayerSubsystem* Subsystem = ULocalPlayer::GetSubsystem<UEnhancedInputLocalPlayerSubsystem>(GetLocalPlayer()))
		{
			for (UInputMappingContext* Context : DefaultMappingContexts) Subsystem->AddMappingContext(Context, 0);
			for (UInputMappingContext* Context : MobileExcludedMappingContexts) Subsystem->AddMappingContext(Context, 0);
		}
	}
}

void AHorrorPlayerController::InitCurrentTime()
{
	CurrentHour = 20;
	CurrentMinute = 0.0f;
	ElapsedTime = 0.0f;
}

/* ===============================
   🎬 Cinematic System Implementation
   =============================== */

void AHorrorPlayerController::StartCinematic_Implementation(ALevelSequenceActor* SequenceActor, ACineCameraActor* CameraActor)
{
	if (!SequenceActor || !CameraActor || !CinematicPawnClass) return;

	bIsPaused = true;
	OriginalPawn = GetPawn();

	// 1. 시네마틱 모드 (입력 차단)
	SetCinematicMode(true, true, true, true, true);

	UHeadMountedDisplayFunctionLibrary::ResetOrientationAndPosition();

	FActorSpawnParameters SpawnParams;
	SpawnParams.SpawnCollisionHandlingOverride = ESpawnActorCollisionHandlingMethod::AlwaysSpawn;
	CinematicPawnInstance = GetWorld()->SpawnActor<APawn>(CinematicPawnClass, CameraActor->GetActorTransform(), SpawnParams);

	if (CinematicPawnInstance) Possess(CinematicPawnInstance);

	SetViewTargetWithBlend(SequenceActor, 0.0f);

	if (ULevelSequencePlayer* SeqPlayer = SequenceActor->GetSequencePlayer())
	{
		SeqPlayer->OnFinished.RemoveAll(this);
		SeqPlayer->OnFinished.AddDynamic(this, &AHorrorPlayerController::Internal_OnCinematicFinished);
		SeqPlayer->Play();
	}
}

void AHorrorPlayerController::Internal_OnCinematicFinished()
{
	ICinematicControlInterface::Execute_EndCinematic(this);
}

void AHorrorPlayerController::EndCinematic_Implementation()
{
	// 1. 원래 몸으로 영혼 복귀
	if (OriginalPawn)
	{
		Possess(OriginalPawn);
		SetViewTargetWithBlend(OriginalPawn, 0.5f);
	}

	// 2. 임시 폰 제거
	if (CinematicPawnInstance)
	{
		CinematicPawnInstance->Destroy();
		CinematicPawnInstance = nullptr;
	}

	// 3. [핵심] 모든 입력 제한 강제 해제 및 명시적 입력 복구
	SetCinematicMode(false, false, false, false, false);
	ResetIgnoreInputFlags(); // 컨트롤러에 걸린 모든 무시 플래그 리셋

	// 이동 및 회전 무시 플래그를 명시적으로 해제합니다.
	SetIgnoreMoveInput(false);
	SetIgnoreLookInput(false);

	// 4. [핵심] 입력 모드를 게임 전용으로 명시적 전환
	FInputModeGameOnly InputMode;
	SetInputMode(InputMode);

	bIsPaused = false;

	UE_LOG(LogTemp, Warning, TEXT("Cinematic Ended: Input, Movement, and Look Restored."));
}