#include "Variant_Horror/HorrorCharacter.h" // 헤더 경로 유지
#include "Engine/World.h"
#include "TimerManager.h"
#include "Kismet/GameplayStatics.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "Camera/CameraComponent.h"
#include "Components/SpotLightComponent.h"
#include "EnhancedInputComponent.h"
#include "InputAction.h"
#include "UObject/ConstructorHelpers.h" // FObjectFinder 사용 시 필수
#include "Components/AudioComponent.h"
#include "HorrorPlayerController.h"
#include "Sound/SoundCue.h"

bool AHorrorCharacter::GetSprintState() const
{
	return bSprinting;
}

AHorrorCharacter::AHorrorCharacter()
{
	// create the spotlight
	SpotLight = CreateDefaultSubobject<USpotLightComponent>(TEXT("SpotLight"));
	SpotLight->SetupAttachment(GetFirstPersonCameraComponent());

	SpotLight->SetRelativeLocationAndRotation(FVector(30.0f, 17.5f, -5.0f), FRotator(-18.6f, -1.3f, 5.26f));
	SpotLight->Intensity = 0.5;
	SpotLight->SetIntensityUnits(ELightUnits::Lumens);
	SpotLight->AttenuationRadius = 1050.0f;
	SpotLight->InnerConeAngle = 18.7f;
	SpotLight->OuterConeAngle = 45.24f;

	GetCharacterMovement()->NavAgentProps.bCanCrouch = true;
	// 앉았을 때 이동 속도 조절
	GetCharacterMovement()->MaxWalkSpeedCrouched = 200.0f;

	// 기본 HP값 설정
	MaxHP = 100.0f;
	CurrentHP = MaxHP;
	bIsDead = false;
	SprintMeter = SprintTime;
	ChasingEnemyCount = 0;

	//// 생성자에서 사운드 큐 파일 찾아서 로드하기 (Hard Loading)
	//static ConstructorHelpers::FObjectFinder<USoundBase> DamageSoundAsset(TEXT("/Game/EscapeTheOvertime/07_FX/SFX/CinematicSound/Ugh_Cue.Ugh_Cue"));

	//if (DamageSoundAsset.Succeeded())
	//{
	//	DamageSound = DamageSoundAsset.Object;
	//}

	//// 심장박동 오디오 컴포넌트 설정
	//HeartbeatAudioComponent = CreateDefaultSubobject<UAudioComponent>(TEXT("HeartbeatAudioComponent"));
	//HeartbeatAudioComponent->SetupAttachment(RootComponent);
	//HeartbeatAudioComponent->bAutoActivate = false; // 시작 시 자동 재생 끔

	//// 심장박동 사운드 큐 로드
	//static ConstructorHelpers::FObjectFinder<USoundCue> HeartbeatCueAsset(TEXT("/Game/EscapeTheOvertime/06_Audio/BGM/Player/HeartBeat_Cue.HeartBeat_Cue"));
	//if (HeartbeatCueAsset.Succeeded())
	//{
	//	HeartbeatAudioComponent->SetSound(HeartbeatCueAsset.Object);
	//}
}

void AHorrorCharacter::BeginPlay()
{
	Super::BeginPlay();

	// initialize sprint meter to max
	SprintMeter = SprintTime;

	// 게임 시작 시 HP 초기화
	CurrentHP = MaxHP;
	bIsDead = false;
	
	// 시작 시 장소 초기화
	CurrentDeathLocation = EDeathLocationType::Window;

	// UI가 초기 상태(100%)를 그릴 수 있도록 방송
	if (MaxHP > 0.0f)
	{
		OnHealthChanged.Broadcast(CurrentHP / MaxHP);
	}

	// Initialize the walk speed
	GetCharacterMovement()->MaxWalkSpeed = WalkSpeed;

	// start the sprint tick timer
	GetWorld()->GetTimerManager().SetTimer(SprintTimer, this, &AHorrorCharacter::SprintFixedTick, SprintFixedTickTime, true);
}

void AHorrorCharacter::EndPlay(EEndPlayReason::Type EndPlayReason)
{
	Super::EndPlay(EndPlayReason);

	// clear the sprint timer
	GetWorld()->GetTimerManager().ClearTimer(SprintTimer);
}
/*
void AHorrorCharacter::UpdateHeartbeatState(bool bIsDetected)
{
	// 1. 추격자 수 카운팅
	if (bIsDetected)
	{
		ChasingEnemyCount++;
	}
	else
	{
		// 0 미만 방지
		ChasingEnemyCount = FMath::Max(0, ChasingEnemyCount - 1);
	}

	// 2. 재생 로직 (Fade In/Out)
	if (ChasingEnemyCount > 0)
	{
		// 쫓는 적이 1명이라도 있고, 소리가 안 나고 있다면 재생 시작
		if (!HeartbeatAudioComponent->IsPlaying())
		{
			// 0.5초 동안 볼륨 0 -> 1로 부드럽게 재생
			HeartbeatAudioComponent->FadeIn(0.5f, 1.0f);
			// UE_LOG(LogTemp, Warning, TEXT("Heartbeat Start! (Chasers: %d)"), ChasingEnemyCount);
		}
	}
	else
	{
		// 쫓는 적이 아무도 없으면 중지
		if (HeartbeatAudioComponent->IsPlaying())
		{
			// 1.5초 동안 볼륨 1 -> 0으로 부드럽게 중지
			HeartbeatAudioComponent->FadeOut(1.5f, 0.0f);
			// UE_LOG(LogTemp, Warning, TEXT("Heartbeat Stop. (Safe)"));
		}
	}
}
*/

// 보스가 때리면 자동 호출됨. 맞았을 때만 UI 업데이트 방송을 함.
float AHorrorCharacter::TakeDamage(float DamageAmount, FDamageEvent const& DamageEvent, AController* EventInstigator, AActor* DamageCauser)
{
	// 이미 죽었거나 데미지가 없으면 무시
	if (bIsDead || DamageAmount <= 0.0f)
	{
		return 0.0f;
	}

	float ActualDamage = Super::TakeDamage(DamageAmount, DamageEvent, EventInstigator, DamageCauser);

	// HP 감소 및 0 미만 방지
	CurrentHP = FMath::Clamp(CurrentHP - ActualDamage, 0.0f, MaxHP);

	//  맞았을 때만 UI에게 "나 체력 변했어!"라고 알림
	if (MaxHP > 0.0f)
	{
		OnHealthChanged.Broadcast(CurrentHP / MaxHP);
	}

	// 데미지를 입었으면 소리 재생 (2D)
	if (ActualDamage > 0.0f && DamageSound)
	{
		// 플레이어 본인의 소리이므로 위치값 없이 2D로 재생 (헤드셋 전체에서 들림)
		UGameplayStatics::PlaySound2D(this, DamageSound);
	}

	// 사망 체크
	if (CurrentHP <= 0.0f && !bIsGameCleared) //player dies if game is not cleared
	{
		OnDeath();
	}

	return ActualDamage;
}

// 사망 처리 (C++)
void AHorrorCharacter::OnDeath_Implementation()
{
	if (bIsDead || bIsGameCleared) return; //returns when the player is already dead or the game is cleared;

	bIsDead = true;

	// 입력 막기 (플레이어 멈춤)
	if (APlayerController* PC = Cast<APlayerController>(GetController()))
	{
		DisableInput(PC);
	}

	if (AHorrorPlayerController* PC = Cast<AHorrorPlayerController>(GetController()))
	{
		if (PC->GetTimeLimitReached())
		{
			CurrentDeathLocation = EDeathLocationType::TimeOver;
			// 하이브리드 연결: 현재 저장된 위치 태그(CurrentDeathLocation)를 블루프린트로 넘김
			PlayGameOverCinematic(CurrentDeathLocation);
			UE_LOG(LogTemp, Warning, TEXT("Player Died. Actor Name %s Calling Cinematic Logic, Location %d"), *GetName(), CurrentDeathLocation);
		}
		else
		{
			StunPlayer();
			UE_LOG(LogTemp, Warning, TEXT("Player is stunned for 5 sec"));
		}
	}
}	

void AHorrorCharacter::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);

	{
		// Set up action bindings
		if (UEnhancedInputComponent* EnhancedInputComponent = Cast<UEnhancedInputComponent>(PlayerInputComponent))
		{
			// Sprinting
			EnhancedInputComponent->BindAction(SprintAction, ETriggerEvent::Started, this, &AHorrorCharacter::DoStartSprint);
			EnhancedInputComponent->BindAction(SprintAction, ETriggerEvent::Completed, this, &AHorrorCharacter::DoEndSprint);

			if (CrouchAction)
			{
				EnhancedInputComponent->BindAction(CrouchAction, ETriggerEvent::Started, this, &AHorrorCharacter::StartCrouch);
				EnhancedInputComponent->BindAction(CrouchAction, ETriggerEvent::Completed, this, &AHorrorCharacter::StopCrouch);
			}
		}
	}
}

// 위치 태그 갱신 함수 (트리거 박스에서 호출)
void AHorrorCharacter::SetDeathLocation(EDeathLocationType NewLocation)
{
	CurrentDeathLocation = NewLocation;
	// 디버깅용 로그 (필요시 주석 해제)
	// UE_LOG(LogTemp, Log, TEXT("Death Location Updated: %d"), (int32)NewLocation);
}

// 앉기 시작
void AHorrorCharacter::StartCrouch(const FInputActionValue& Value)
{
	Crouch();
}

// 앉기 종료 (일어서기)
void AHorrorCharacter::StopCrouch(const FInputActionValue& Value)
{
	UnCrouch();
}

void AHorrorCharacter::DoStartSprint()
{
	// 죽었으면 달리기 불가
	if (bIsDead) return;

	// set the sprinting flag
	bSprinting = true;

	// are we out of recovery mode?
	if (!bRecovering)
	{
		// set the sprint walk speed
		//if (!bIsSpedUp)
		//	GetCharacterMovement()->MaxWalkSpeed = SprintSpeed;
		//else
		//	GetCharacterMovement()->MaxWalkSpeed = IncreasedSpeed;

		GetCharacterMovement()->MaxWalkSpeed = bIsSpedUp ? IncreasedSpeed : SprintSpeed;

		// call the sprint state changed delegate
		OnSprintStateChanged.Broadcast(true);
	}
}

void AHorrorCharacter::DoEndSprint()
{
	// set the sprinting flag
	bSprinting = false;

	// are we out of recovery mode?
	if (!bRecovering)
	{
		// set the default walk speed
		GetCharacterMovement()->MaxWalkSpeed = bIsSpedUp? SprintSpeed : WalkSpeed;

		// call the sprint state changed delegate
		OnSprintStateChanged.Broadcast(false);
	}
}

void AHorrorCharacter::SprintFixedTick()
{
	// 죽었으면 스태미나 로직 중단
	if (bIsDead) return;

	// are we out of recovery, still have stamina and are moving faster than our walk speed?
	if (bSprinting && !bRecovering && GetVelocity().Length() > WalkSpeed)
	{
		// do we still have meter to burn?
		if (SprintMeter > 0.0f)
		{
			// update the sprint meter
			if (!bIsSpedUp)
				SprintMeter = FMath::Max(SprintMeter - SprintFixedTickTime, 0.0f);
			else
				SprintMeter = FMath::Min(SprintMeter + SprintFixedTickTime, SprintTime);

			// have we run out of stamina?
			if (SprintMeter <= 0.0f)
			{
				// raise the recovering flag
				bRecovering = true;

				// set the recovering walk speed
				GetCharacterMovement()->MaxWalkSpeed = RecoveringWalkSpeed;

			}
		}
	}
	else
	{
		// recover stamina
		SprintMeter = FMath::Min(SprintMeter + SprintFixedTickTime, SprintTime);

		if (SprintMeter >= SprintTime)
		{
			// lower the recovering flag
			bRecovering = false;

			// set the walk or sprint speed depending on whether the sprint button is down
			if (!bIsSpedUp)
				GetCharacterMovement()->MaxWalkSpeed = bSprinting ? SprintSpeed : WalkSpeed;
			else
				GetCharacterMovement()->MaxWalkSpeed = bSprinting ? IncreasedSpeed : SprintSpeed;

			// update the sprint state depending on whether the button is down or not
			OnSprintStateChanged.Broadcast(bSprinting);
		}
	}

	// broadcast the sprint meter updated delegate
	OnSprintMeterUpdated.Broadcast(SprintMeter / SprintTime);
}

void AHorrorCharacter::StunPlayer()
{
	bIsDead = true;

    // 1. 화면 서서히 암전 (1초 소요)
    APlayerController* PC = Cast<APlayerController>(GetController());
    if (PC && PC->PlayerCameraManager)
    {
        PC->PlayerCameraManager->StartCameraFade(0.0f, 1.0f, 1.0f, FLinearColor::Black, false, true);
    }

    // 2. 5초 뒤에 RecoverFromStun 함수 실행 (기절 지속 시간)
    GetWorldTimerManager().SetTimer(StunTimerHandle, this, &AHorrorCharacter::RecoverFromStun, 5.0f, false);
}

void AHorrorCharacter::RecoverFromStun()
{
	bIsDead = false;

    CurrentHP = MaxHP; // 체력 회복

    // 3. 화면 다시 밝아짐 (검은색 -> 투명, 1초 소요)
    APlayerController* PC = Cast<APlayerController>(GetController());
    if (PC)
    {
        if (PC->PlayerCameraManager)
        {
            PC->PlayerCameraManager->StartCameraFade(1.0f, 0.0f, 1.0f, FLinearColor::Black, false, false);
        }
        
        // 기절 해제 시 입력을 다시 활성화
        EnableInput(PC);
    }

	UE_LOG(LogTemp, Warning, TEXT("Player Recovered from Stun. Health Restored to %.1f"), CurrentHP);

    // UI 업데이트
    OnHealthChanged.Broadcast(CurrentHP / MaxHP);
}

//void AHorrorCharacter::ActivateSpeedUp()
//{
//	// 부모의 로직(타이머 설정, 로그 등)은 그대로 실행하고 싶다면:
//	Super::ActivateSpeedUp();
//
//	// 하지만 속도 설정만큼은 내 방식(변수 사용)대로 확실하게 다시 잡는다!
//	// 내가 아까 만들어준 함수를 여기서 호출하거나, 로직을 직접 씀
//	SetSpeedUpState(true);
//}

//void AHorrorCharacter::SetSpeedUpState(bool bNewState)
//{
//	// 상태가 이전과 같다면 아무것도 하지 않음 (불필요한 연산 방지)
//	if (bIsSpedUp == bNewState)
//	{
//		return;
//	}
//
//	// 1. 상태 변수 업데이트
//	bIsSpedUp = bNewState;
//
//	UE_LOG(LogTemp, Warning, TEXT("SpeedUp State Changed: %s"), bIsSpedUp ? TEXT("ON") : TEXT("OFF"));
//
//	// 2. 현재 상황에 맞춰 즉시 이동 속도(MaxWalkSpeed) 갱신
//
//	if (bRecovering)
//	{
//		// A. 탈진(Recovering) 상태일 때
//		// 보통 탈진 상태에서는 버프가 있어도 느리게 걷습니다. (기획에 따라 다를 수 있음)
//		GetCharacterMovement()->MaxWalkSpeed = RecoveringWalkSpeed;
//	}
//	else if (bSprinting)
//	{
//		// B. 달리기(Shift 누름) 중일 때
//		// 버프 상태면 IncreasedSpeed, 아니면 일반 SprintSpeed
//		GetCharacterMovement()->MaxWalkSpeed = bIsSpedUp ? IncreasedSpeed : SprintSpeed;
//	}
//	else
//	{
//		// C. 걷기(Shift 안 누름) 중일 때
//		// 네 코드 로직(DoEndSprint)에 따르면: 버프 상태일 때 걷는 속도는 일반 달리기 속도(SprintSpeed)와 같음
//		GetCharacterMovement()->MaxWalkSpeed = bIsSpedUp ? SprintSpeed : WalkSpeed;
//	}
//}

void AHorrorCharacter::Heal(float Amount)
{
	CurrentHP = FMath::Min(CurrentHP + Amount, MaxHP);
	OnHealthChanged.Broadcast(CurrentHP / MaxHP);
	UE_LOG(LogTemp, Warning, TEXT("Player Healed by %.1f, Health: %.1f"), Amount, CurrentHP);
}

void AHorrorCharacter::AddMaxHP(float Amount)
{
	MaxHP += Amount;
	CurrentHP = FMath::Min(CurrentHP + Amount, MaxHP);
	OnHealthChanged.Broadcast(CurrentHP / MaxHP);
	UE_LOG(LogTemp, Warning, TEXT("MaxHP Increased by %.1f, Health: %.1f / %.1f"), Amount, CurrentHP, MaxHP);
}

void AHorrorCharacter::SetHP(float Value)
{
	MaxHP = Value;
	CurrentHP = MaxHP;
	UE_LOG(LogTemp, Warning, TEXT("HP is now %f Max"), MaxHP);
}

//void AHorrorCharacter::DamagePlayer(float Amount)
//{
//	CurrentHP = FMath::Clamp(CurrentHP - Amount, 0.0f, MaxHP);
//	OnHealthChanged.Broadcast(CurrentHP / MaxHP);
//	UE_LOG(LogTemp, Warning, TEXT("Player Damaged by %.1f, Health: %.1f"), Amount, CurrentHP);
//}

