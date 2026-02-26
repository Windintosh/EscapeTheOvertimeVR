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

	// VR 트래킹 오리진 설정
	UHeadMountedDisplayFunctionLibrary::SetTrackingOrigin(EHMDTrackingOrigin::Stage);

	// 모바일 컨트롤 UI 생성
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

	// 인게임 시간 흐름 계산 (20시 시작 ~ 24시 종료)
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

	// 캐릭터 빙의 시 메인 HUD UI 생성 및 데이터 연결
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

	// Enhanced Input 컨텍스트 등록
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
	* @param bAllowInput 체크 시 시네마틱 도중에도 캐릭터 조작(버튼 클릭 등)이 가능합니다.
	*/
void AHorrorPlayerController::StartCinematic_Implementation(ALevelSequenceActor* SequenceActor, ACineCameraActor* CameraActor, bool bAllowInput)
{
	if (!SequenceActor || !CameraActor || !CinematicPawnClass) return;

	bIsPaused = true;
	OriginalPawn = GetPawn();

	// 1. 시네마틱 모드 설정
	// bAllowInput이 true이면 bAffectsMovement를 false로 전달하여 입력을 허용함
	SetCinematicMode(true, true, true, !bAllowInput, true);

	// 명시적으로 이동/회전 입력 제어
	if (bAllowInput)
	{
		SetIgnoreMoveInput(false);
		SetIgnoreLookInput(true); // 시점은 시네마틱 카메라에 고정하는 것이 VR에서 안정적임
	}

	UHeadMountedDisplayFunctionLibrary::ResetOrientationAndPosition();

	// 2. 유체이탈용 시네마틱 폰 스폰 및 빙의
	FActorSpawnParameters SpawnParams;
	SpawnParams.SpawnCollisionHandlingOverride = ESpawnActorCollisionHandlingMethod::AlwaysSpawn;
	CinematicPawnInstance = GetWorld()->SpawnActor<APawn>(CinematicPawnClass, CameraActor->GetActorTransform(), SpawnParams);

	if (CinematicPawnInstance) Possess(CinematicPawnInstance);

	// 3. 뷰 타겟 전환 (시퀀서 카메라 컷 연동)
	SetViewTargetWithBlend(SequenceActor, 0.0f);

	// 4. 시퀀스 재생 및 종료 이벤트 바인딩
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
		SetViewTargetWithBlend(OriginalPawn, 0.5f); // 부드럽게 복귀
	}

	// 2. 임시 폰 제거
	if (CinematicPawnInstance)
	{
		CinematicPawnInstance->Destroy();
		CinematicPawnInstance = nullptr;
	}

	// 3. 모든 입력 제한 강제 해제 및 상태 리셋
	SetCinematicMode(false, false, false, false, false);
	ResetIgnoreInputFlags();

	SetIgnoreMoveInput(false);
	SetIgnoreLookInput(false);

	// 4. 입력 모드를 게임 전용으로 명시적 전환 (UI 조작 잔상 방지)
	FInputModeGameOnly InputMode;
	SetInputMode(InputMode);

	bIsPaused = false;

	// 맵 전환 및 리소스 정리를 위한 명령 실행
	UKismetSystemLibrary::ExecuteConsoleCommand(GetWorld(), TEXT("r.FlushRenderingCommands"));

	UE_LOG(LogTemp, Warning, TEXT("Cinematic Ended: Input, Movement, and Look Restored. Rendering Flushed."));
}