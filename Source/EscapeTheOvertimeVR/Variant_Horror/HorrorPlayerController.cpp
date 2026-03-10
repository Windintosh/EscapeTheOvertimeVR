// Copyright Epic Games, Inc. All Rights Reserved.

#include "Variant_Horror/HorrorPlayerController.h"
#include "EnhancedInputComponent.h"
#include "EnhancedInputSubsystems.h"
#include "LevelSequenceActor.h"
#include "LevelSequencePlayer.h"
#include "CineCameraActor.h"
#include "HeadMountedDisplayFunctionLibrary.h"
#include "Blueprint/UserWidget.h"
#include "HorrorCharacter.h"
#include "HorrorUI.h"
#include "Kismet/GameplayStatics.h"
#include "MainClass/Characters/VRCharacterPawn/VRCharacterPawn.h"

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

	// 게임 시작 후 0.5초 뒤 자동 영점 조절 (트래킹 시스템 초기화 대기)
	GetWorld()->GetTimerManager().SetTimer(RecenterTimerHandle, this, &AHorrorPlayerController::RecenterVR, 0.5f, false);

	// 모바일 컨트롤 UI 생성
	// if (IsLocalPlayerController() && MobileControlsWidgetClass)
	// {
	// 	MobileControlsWidget = CreateWidget<UUserWidget>(this, MobileControlsWidgetClass);
	// 	if (MobileControlsWidget) MobileControlsWidget->AddToPlayerScreen(0);
	// }
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
			bIsTimeLimitReached = true;
			CurrentHour = 0;
			if (AHorrorCharacter* MyChar = Cast<AHorrorCharacter>(GetPawn())) 
			{
				MyChar->SetDeathLocation(EDeathLocationType::TimeOver);
				MyChar->OnDeath();
			}
			OnTimeLimitReached();
		}
	}
}

void AHorrorPlayerController::OnPossess(APawn* aPawn)
{
	Super::OnPossess(aPawn);

	// 캐릭터 빙의 시 메인 HUD UI 생성 및 데이터 연결
	// if (IsLocalPlayerController() && aPawn && HorrorUIClass)
	// {
	// 	if (!HorrorUI)
	// 	{
	// 		HorrorUI = CreateWidget<UHorrorUI>(this, HorrorUIClass);
	// 		if (HorrorUI) HorrorUI->AddToViewport(0);
	// 	}
	// 	if (AHorrorCharacter* HorrorCharacter = Cast<AHorrorCharacter>(aPawn))
	// 	{
	// 		if (HorrorUI) HorrorUI->SetupCharacter(HorrorCharacter);
	// 	}
	// }
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

	// 입력 액션 바인딩
	if (UEnhancedInputComponent* EnhancedInputComponent = Cast<UEnhancedInputComponent>(InputComponent))
	{
		if (UtilityAction)
		{
			EnhancedInputComponent->BindAction(UtilityAction, ETriggerEvent::Triggered, this, &AHorrorPlayerController::OnUtilityActionPressed);
		}
	}
}

void AHorrorPlayerController::OnUtilityActionPressed(const FInputActionValue& Value)
{
	if (ActiveSequenceActor)
	{
		SkipCurrentCinematic();
	}
	else
	{
		RecenterVR();
	}
}

void AHorrorPlayerController::RecenterVR()
{
	UHeadMountedDisplayFunctionLibrary::ResetOrientationAndPosition(0.0f, EOrientPositionSelector::OrientationAndPosition);
	UE_LOG(LogTemp, Log, TEXT("VR Tracking Recentered."));
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
	ActiveSequenceActor = SequenceActor;

	// 1. 시네마틱 모드 설정
	// bAllowInput이 true이면 bAffectsMovement를 false로 전달하여 입력을 허용함
	SetCinematicMode(true, true, true, !bAllowInput, true);

	// 명시적으로 이동/회전 입력 제어
	if (bAllowInput)
	{
		SetIgnoreMoveInput(false);
		SetIgnoreLookInput(true); // 시점은 시네마틱 카메라에 고정하는 것이 VR에서 안정적임
	}

	// [수정] VR 트래킹 파괴 방지
	// UHeadMountedDisplayFunctionLibrary::ResetOrientationAndPosition(); 를 호출하면
	// 방 크기와 센서 캘리브레이션 영점이 초기화되어 시네마틱 이후 바닥/공중에 뜨는 버그가 발생합니다. 호출하지 마세요.

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

void AHorrorPlayerController::SkipCurrentCinematic()
{
	if (ActiveSequenceActor)
	{
		ALevelSequenceActor* TempSeqActor = ActiveSequenceActor;
		// 1. 중복 실행 방지를 위해 먼저 주소를 날립니다.
		ActiveSequenceActor = nullptr;

		if (ULevelSequencePlayer* SeqPlayer = TempSeqActor->GetSequencePlayer())
		{
			// 2. 엔진 자체 이벤트를 해제하여 OnFinished가 두 번 연쇄 폭발하는 것을 막습니다.
			// (이 함수는 현재 C++ 클래스의 바인딩만 해제하며, 블루프린트에 연결된 이벤트는 건드리지 않습니다.)
			SeqPlayer->OnFinished.RemoveAll(this);

			// 3. 엔진 시퀀서를 끝 상태로 강제 스냅하여 애니메이션(문, 조명 등)을 최종 상태로 맞춥니다.
			// (주의: 언리얼 엔진 구조상 GoToEndAndStop()은 OnFinished 이벤트를 발생시키지 않습니다)
			SeqPlayer->GoToEndAndStop();
			
			// 4. [가장 중요] "스킵용 블루프린트 트리거"보다 먼저 수행되어야 할 최우선 작업: 원래 몸 복구!
			// 여기서 임시 시네마틱 카메라를 폭파하고 VR 폰으로 안전하게 돌아옵니다.
			ICinematicControlInterface::Execute_EndCinematic(this);

			// 5. 트리거/레벨 블루프린트에 연결된 스킵 후속 처리(위치 이동, 사운드 재생)를 강제 발동시킵니다!
			// 4번 과정 덕분에, OnFinished 블루프린트 노드에서 GetPlayerPawn()을 부를 때 임시 카메라폰이 아닌 "진짜 VR폰"이 반환되어 공간 점프가 완벽히 먹힙니다.
			SeqPlayer->OnFinished.Broadcast();
		}
	}
}

void AHorrorPlayerController::EndCinematic_Implementation()
{
	// 1. 원래 몸으로 영혼 복귀
	if (OriginalPawn)
	{
		// 시네마틱 카메라가 남긴 Pitch/Roll 값(위아래로 꺾인 시야각)을 0으로 초기화하지 않으면
		// VR의 VROrigin 높이나 방향이 꼬여서 키가 비정상적으로 커지거나 작아집니다.
		FRotator FlatRotation(0.0f, OriginalPawn->GetActorRotation().Yaw, 0.0f);
		SetControlRotation(FlatRotation);

		Possess(OriginalPawn);
		SetViewTargetWithBlend(OriginalPawn, 0.0f); // VR에서는 카메라 블렌딩이 어지러움과 위치 오류를 유발할 수 있으므로 0초로 즉시 전환
	}

	// 2. 임시 폰 제거
	if (CinematicPawnInstance)
	{
		CinematicPawnInstance->Destroy();
		CinematicPawnInstance = nullptr;
	}

	// 3. 모든 입력 제한 강제 해제 및 상태 리셋
	// [중요] 마지막 두 매개변수(bAffectsMovement, bAffectsTurning)를 true로 넘겨야 엔진이 카운터를 정상적으로 마이너스(해제)시킵니다.
	SetCinematicMode(false, false, false, true, true); 
	ResetIgnoreInputFlags();

	SetIgnoreMoveInput(false);
	SetIgnoreLookInput(false);

	// 강제 인풋 잠금 해제 (완전 보장)
	if (OriginalPawn)
	{
		OriginalPawn->EnableInput(this);
	}
	this->EnableInput(this);

	// 4. 입력 모드를 게임 전용으로 명시적 전환 (UI 조작 잔상 방지)
	FInputModeGameOnly InputMode;
	SetInputMode(InputMode);

	bIsPaused = false;
	ActiveSequenceActor = nullptr;

	// Enhanced Input 컨텍스트가 빙의 복구 과정에서 날아갔을 수 있으므로 다시 주입 (이동/회전 버그 픽스)
	if (IsLocalPlayerController())
	{
		if (UEnhancedInputLocalPlayerSubsystem* Subsystem = ULocalPlayer::GetSubsystem<UEnhancedInputLocalPlayerSubsystem>(GetLocalPlayer()))
		{
			// 컨트롤러 자체 입력 복구
			for (UInputMappingContext* Context : DefaultMappingContexts) Subsystem->AddMappingContext(Context, 0);
			for (UInputMappingContext* Context : MobileExcludedMappingContexts) Subsystem->AddMappingContext(Context, 0);
			
			// VR 폰 개별 매핑 컨텍스트가 BeginPlay에서 주입되었으므로 따로 복구
			if (AVRCharacterPawn* VRPawn = Cast<AVRCharacterPawn>(OriginalPawn))
			{
				if (VRPawn->DefaultMappingContext)
				{
					Subsystem->AddMappingContext(VRPawn->DefaultMappingContext, 0);
				}
			}
		}
	}

	// 맵 전환 및 리소스 정리를 위한 명령 실행
	UKismetSystemLibrary::ExecuteConsoleCommand(GetWorld(), TEXT("r.FlushRenderingCommands"));

	UE_LOG(LogTemp, Warning, TEXT("Cinematic Ended: Input, Movement, and Look Restored. Rendering Flushed."));
}