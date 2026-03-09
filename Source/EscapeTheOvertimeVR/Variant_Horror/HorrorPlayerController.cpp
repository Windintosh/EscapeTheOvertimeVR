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
#include "Kismet/KismetSystemLibrary.h"

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

   /**
	* 이제 파라미터에서 bool 값이 제거되었습니다.
	* 모든 시네마틱에서 항상 인풋을 허용하고 시점만 고정합니다.
	*/
void AHorrorPlayerController::StartCinematic_Implementation(ALevelSequenceActor* SequenceActor, ACineCameraActor* CameraActor)
{
	if (!SequenceActor || !CameraActor || !CinematicPawnClass) return;

	bIsPaused = true;
	OriginalPawn = GetPawn();
	ActiveSequenceActor = SequenceActor;

	// 1. 시네마틱 모드 설정
	// 네 번째 인자(bAffectsMovement)를 false로 강제 설정하여 항상 키 입력을 허용합니다.
	// 다섯 번째 인자(bAffectsLook)를 true로 설정하여 시점은 카메라에 고정합니다.
	SetCinematicMode(true, true, true, false, true);

	// 2. 명시적 인풋 상태 강제 (이동 허용, 시선 고정)
	SetIgnoreMoveInput(false);
	SetIgnoreLookInput(true);

	UHeadMountedDisplayFunctionLibrary::ResetOrientationAndPosition();

	// 3. 임시 폰 스폰 및 빙의 (시점 고정용)
	FActorSpawnParameters SpawnParams;
	SpawnParams.SpawnCollisionHandlingOverride = ESpawnActorCollisionHandlingMethod::AlwaysSpawn;
	CinematicPawnInstance = GetWorld()->SpawnActor<APawn>(CinematicPawnClass, CameraActor->GetActorTransform(), SpawnParams);

	if (CinematicPawnInstance) Possess(CinematicPawnInstance);

	// 4. 시점 전환 (0.0초 즉시)
	SetViewTargetWithBlend(SequenceActor, 0.0f);

	if (ULevelSequencePlayer* SeqPlayer = SequenceActor->GetSequencePlayer())
	{
		SeqPlayer->OnFinished.RemoveAll(this);
		SeqPlayer->OnFinished.AddDynamic(this, &AHorrorPlayerController::Internal_OnCinematicFinished);
		SeqPlayer->Play();
	}
}

void AHorrorPlayerController::SkipCurrentCinematic()
{
	if (ActiveSequenceActor)
	{
		if (ULevelSequencePlayer* SeqPlayer = ActiveSequenceActor->GetSequencePlayer())
		{
			SeqPlayer->GoToEndAndStop();
			UE_LOG(LogTemp, Warning, TEXT("Cinematic Skipped via SkipCurrentCinematic()."));
		}
	}
}

void AHorrorPlayerController::Internal_OnCinematicFinished()
{
	ICinematicControlInterface::Execute_EndCinematic(this);
}

void AHorrorPlayerController::EndCinematic_Implementation()
{
	if (OriginalPawn)
	{
		Possess(OriginalPawn);
		SetViewTargetWithBlend(OriginalPawn, 0.5f);
	}

	if (CinematicPawnInstance)
	{
		CinematicPawnInstance->Destroy();
		CinematicPawnInstance = nullptr;
	}

	ActiveSequenceActor = nullptr;

	// 모든 모드 및 입력 제한 완전 해제
	SetCinematicMode(false, false, false, false, false);
	ResetIgnoreInputFlags();
	SetIgnoreMoveInput(false);
	SetIgnoreLookInput(false);

	FInputModeGameOnly InputMode;
	SetInputMode(InputMode);

	bIsPaused = false;
	UKismetSystemLibrary::ExecuteConsoleCommand(GetWorld(), TEXT("r.FlushRenderingCommands"));
}