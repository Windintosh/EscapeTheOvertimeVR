#include "MainClass/Characters/VRCharacterPawn/VRCharacterPawn.h"
#include "Engine/OverlapResult.h" // 충돌 결과 처리용
#include "DrawDebugHelpers.h"     // 디버그 원 그리기용
#include "Camera/CameraComponent.h"
#include "GameFramework/CharacterMovementComponent.h" // 필수 헤더
#include "Components/CapsuleComponent.h"
#include "ThrownItem.h"
#include "Components/SkeletalMeshComponent.h"

// Sets default values
AVRCharacterPawn::AVRCharacterPawn()
{
 	// Set this pawn to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	// 1. 부모(FPS)의 메쉬 숨기기
	// VR에서는 내 몸통이 보이면 시야를 가리거나 이상하게 보일 수 있습니다.
	// 'SetOwnerNoSee(true)': 나한테는 안 보이고, 거울이나 멀티플레이 상대방에게는 보임.
	if (GetMesh())
	{
		GetMesh()->SetOwnerNoSee(true);
		// 만약 그림자도 거슬리면: GetMesh()->SetCastShadow(false);
	}

	
	// ACharacter는 이미 'GetCapsuleComponent()'가 Root입니다.
	// VR은 캡슐 크기를 좀 작게 하거나 조절할 필요가 있습니다.
	GetCapsuleComponent()->InitCapsuleSize(20.f, 96.0f);


	// 1. 루트 컴포넌트 생성(기준점)->APawn일때만, ACharacter는 이미 루트 컴포넌트가 존재
	VROrigin = CreateDefaultSubobject<USceneComponent>(TEXT("VROrigin"));
	VROrigin->SetupAttachment(RootComponent); // 캡슐(Root) 아래에 붙임
	//RootComponent = VROrigin;

	// 2. 카메라 생성 및 부착
	VRCamera = CreateDefaultSubobject<UCameraComponent>(TEXT("VRCamera"));
	VRCamera->SetupAttachment(VROrigin);

	// ---------------------------------------------------------
	// [핵심] 이제 CharacterMovement 컴포넌트 설정이 가능해집니다!
	// ---------------------------------------------------------
	// 멀미 방지를 위한 VR 이동 설정 (생성자에서 미리 세팅 추천)
	GetCharacterMovement()->MaxWalkSpeed = MWalkSpeed; // 걷기 속도
	GetCharacterMovement()->MaxAcceleration = 20480.0f; // 즉시 가속 (멀미 감소)
	GetCharacterMovement()->BrakingDecelerationWalking = 20480.0f; // 즉시 정지 (멀미 감소)
	GetCharacterMovement()->bUseControllerDesiredRotation = false; // VR은 몸통 회전은 수동으로 함

	// 3. 왼손 컨트롤러 설정
	LeftHandController = CreateDefaultSubobject<UMotionControllerComponent>(TEXT("LeftHandController"));
	LeftHandController->SetupAttachment(VROrigin);
	LeftHandController->SetTrackingSource(EControllerHand::Left); // 왼손 추적 설정

	// [변경] 왼손 메쉬 생성 (Skeletal)
	LeftHandMesh = CreateDefaultSubobject<USkeletalMeshComponent>(TEXT("LeftHandMesh"));
	LeftHandMesh->SetupAttachment(LeftHandController);
	// VR 손은 보통 회전값이 필요할 수 있습니다. (모델에 따라 다름, 일단 -90도 회전 예시)
	LeftHandMesh->SetRelativeRotation(FRotator(0.0f, 0.0f, -90.0f));

	// 4. 오른손 컨트롤러 설정
	RightHandController = CreateDefaultSubobject<UMotionControllerComponent>(TEXT("RightHandController"));
	RightHandController->SetupAttachment(VROrigin);
	RightHandController->SetTrackingSource(EControllerHand::Right); // 오른손 추적 설정

	// [변경] 오른손 메쉬 생성 (Skeletal)
	RightHandMesh = CreateDefaultSubobject<USkeletalMeshComponent>(TEXT("RightHandMesh"));
	RightHandMesh->SetupAttachment(RightHandController);
	RightHandMesh->SetRelativeRotation(FRotator(0.0f, 0.0f, 90.0f));

	DamagePostProcessComp = CreateDefaultSubobject<UPostProcessComponent>(TEXT("PostProcessComponent"));
	DamagePostProcessComp->SetupAttachment(VRCamera);
	DamagePostProcessComp->bUnbound = false; // 카메라에만 적용 (월드 전체가 아닌)
}

// Called when the game starts or when spawned
void AVRCharacterPawn::BeginPlay()
{
	Super::BeginPlay();
	
	// 1. 플레이어 컨트롤러 가져오기
	if (APlayerController* PlayerController = Cast<APlayerController>(Controller))
	{
		// 2. 향상된 입력 로컬 플레이어 서브시스템 가져오기
		if (UEnhancedInputLocalPlayerSubsystem* Subsystem = ULocalPlayer::GetSubsystem<UEnhancedInputLocalPlayerSubsystem>(PlayerController->GetLocalPlayer()))
		{
			// 3. 매핑 컨텍스트 추가 (우선순위 0)
			if (DefaultMappingContext)
			{
				Subsystem->AddMappingContext(DefaultMappingContext, 0);
			}
		}
		
		if (DamageVignetteMaterialBase && DamagePostProcessComp)
		{
			// 포스트 프로세스 머티리얼 인스턴스 생성
			DamageMaterialInstance = UMaterialInstanceDynamic::Create(DamageVignetteMaterialBase, this);
			if (DamageMaterialInstance)
			{
				/*
				// 포스트 프로세스 컴포넌트에 머티리얼 추가
				FWeightedBlendable Blendable;
				Blendable.Object = PostProcessMaterialInstance;
				Blendable.Weight = 1.0f; // 100% 효과
				PostProcessComponent->Settings.WeightedBlendables.Array.Add(Blendable);
				*/
				// 또는 간단히 AddOrUpdateBlendable 함수 사용
				DamageMaterialInstance->SetScalarParameterValue(TEXT("Intensity"), 0.0f);
				DamagePostProcessComp->AddOrUpdateBlendable(DamageMaterialInstance);
			}
		}
		
	}

	// [핵심] 부모가 만든 FPS 카메라 찾아서 끄기
	// 내 VR 카메라는 켜두고, 나머지 모든 카메라를 비활성화합니다.
	TArray<UCameraComponent*> Cameras;
	GetComponents<UCameraComponent>(Cameras);

	for (UCameraComponent* Cam : Cameras)
	{
		// 내가 방금 만든 VR 카메라는 건드리지 않음
		if (Cam == VRCamera) continue;

		// 부모가 만든 카메라는 끕니다.
		Cam->Deactivate();
		Cam->SetActive(false);

		// 렌더링 타겟에서 제외 (확실하게)
		// Cam->DestroyComponent(); // 과격한 방법 (필요 시 사용)
	}
	if (VRCamera)
	{
		VRCamera->Activate();
	}

	if (RightHandMesh)
	{
		LastRightHandLocation = RightHandMesh->GetComponentLocation();
	}

}



float AVRCharacterPawn::TakeDamage(float DamageAmount, FDamageEvent const& DamageEvent, AController* EventInstigator, AActor* DamageCauser)
{
	Super::TakeDamage(DamageAmount, DamageEvent, EventInstigator, DamageCauser);

	// 데미지 비네트 효과 시작
	PlayDamageEffect();

	return DamageAmount;
}


// Called every frame
void AVRCharacterPawn::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	if (RightHandMesh && DeltaTime > 0.0f)
	{
		FVector CurrentLocation = RightHandMesh->GetComponentLocation();

		// 공식: (현재위치 - 이전위치) / 걸린시간 = 속도
		CalculatedRightHandVelocity = (CurrentLocation - LastRightHandLocation) / DeltaTime;

		// 다음 프레임을 위해 현재 위치 저장
		LastRightHandLocation = CurrentLocation;

		// 디버깅용: 틱마다 속도가 잘 나오는지 확인 (너무 많이 뜨면 주석 처리)
		// if(CalculatedRightHandVelocity.Size() > 10.0f)
		//    UE_LOG(LogTemp, Log, TEXT("Tick Velocity: %f"), CalculatedRightHandVelocity.Size());
	}
}

// Called to bind functionality to input
void AVRCharacterPawn::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);

	// CastChecked를 사용하여 EnhancedInputComponent임을 보장
	if (UEnhancedInputComponent* EnhancedInputComponent = CastChecked<UEnhancedInputComponent>(PlayerInputComponent))
	{
		// 바인딩: Triggered, Started, Completed 등 타이밍 선택 가능
		if (GrabLeftAction)
		{
			//EnhancedInputComponent->BindAction(GrabLeftAction, ETriggerEvent::Started, this, &AVRCharacterPawn::OnGrabLeft);
			//EnhancedInputComponent->BindAction(GrabLeftAction, ETriggerEvent::Completed, this, &AVRCharacterPawn::OnGrabLeft); // 같은 함수에서 bool 값으로 분기하거나 함수를 나누거나 선택->나중에 함수 나눌 예정.
			// 버튼을 누르는 순간 -> 달리기 시작
			EnhancedInputComponent->BindAction(GrabLeftAction, ETriggerEvent::Started, this, &AVRCharacterPawn::OnSprintStart);

			// 버튼을 떼는 순간 -> 달리기 종료 (걷기 복귀)
			EnhancedInputComponent->BindAction(GrabLeftAction, ETriggerEvent::Completed, this, &AVRCharacterPawn::OnSprintEnd);
		}

		if (GrabRightAction)
		{
			EnhancedInputComponent->BindAction(GrabRightAction, ETriggerEvent::Started, this, &AVRCharacterPawn::OnGrabRight);
			EnhancedInputComponent->BindAction(GrabRightAction, ETriggerEvent::Completed, this, &AVRCharacterPawn::OnGrabRight);
		}
		// [이동] Triggered: 누르고 있는 동안 계속 실행
		if (VRMoveAction)
			EnhancedInputComponent->BindAction(VRMoveAction, ETriggerEvent::Triggered, this, &AVRCharacterPawn::Move);

		// [회전] Started: 딱 한 번만 실행 (Snap Turn)
		if (VRTurnAction)
			EnhancedInputComponent->BindAction(VRTurnAction, ETriggerEvent::Started, this, &AVRCharacterPawn::Turn);

		// 트리거 바인딩 (Triggered: 누르는 동안 계속 값 전달, 예: 연사)
		if (TriggerLeftAction)
			EnhancedInputComponent->BindAction(TriggerLeftAction, ETriggerEvent::Triggered, this, &AVRCharacterPawn::OnTriggerLeft);

		if (TriggerRightAction)
			EnhancedInputComponent->BindAction(TriggerRightAction, ETriggerEvent::Started, this, &AVRCharacterPawn::OnTriggerRight);

		if (PrevItemAction)
			EnhancedInputComponent->BindAction(PrevItemAction, ETriggerEvent::Started, this, &AVRCharacterPawn::CycleItemPrev);

		if (NextItemAction)
			EnhancedInputComponent->BindAction(NextItemAction, ETriggerEvent::Started, this, &AVRCharacterPawn::CycleItemNext);
	}
}

void AVRCharacterPawn::SpawnAndEquip(TSubclassOf<AActor> ClassToSpawn, USceneComponent* HandMesh, AActor*& HeldActorRef)
{
	if (!ClassToSpawn || !HandMesh || bIsDead) return;

	// 1. 손 위치에서 스폰
	FVector SpawnLoc = HandMesh->GetComponentLocation();
	FRotator SpawnRot = HandMesh->GetComponentRotation();

	FActorSpawnParameters SpawnParams;
	SpawnParams.SpawnCollisionHandlingOverride = ESpawnActorCollisionHandlingMethod::AlwaysSpawn;

	AActor* NewItem = GetWorld()->SpawnActor<AActor>(ClassToSpawn, SpawnLoc, SpawnRot, SpawnParams);

	if (NewItem)
	{
		// 2. 인터페이스 강제 호출하여 손에 부착
		if (NewItem->Implements<UVRGrabInterface>())
		{
			// 변수에 먼저 등록 (중요: 신호 끊김 방지)
			HeldActorRef = NewItem;

			if (AThrownItem* NewThrownItem = Cast<AThrownItem>(NewItem))
			{
				NewThrownItem->bIsSpawnedItem = true;
				NewThrownItem->bIsSpawned = true;
			}

			// Grab 실행 -> 아이템이 알아서 물리 끄고 손에 붙음 (아까 맞춘 소켓 위치로!)
			IVRGrabInterface::Execute_Grab(NewItem, HandMesh);

			UE_LOG(LogTemp, Log, TEXT("Spawned and Equipped: %s"), *NewItem->GetName());
		}
	}
}

void AVRCharacterPawn::GiveTranquilizer()
{
	bHasTranquilizer = true;
}

void AVRCharacterPawn::TryGrabActor(USceneComponent* HandMesh, AActor*& OutHeldActor)
{
	// 1. 이미 뭔가를 잡고 있다면 무시
	if (OutHeldActor || bIsDead) return;

	// 2. 구형 충돌 감지 설정
	FVector HandLocation = HandMesh->GetComponentLocation();
	FCollisionShape Sphere = FCollisionShape::MakeSphere(GrabRadius);

	// 감지할 채널 설정 (PhysicsBody, WorldDynamic 등 잡을 물체의 타입)
	// 보통 잡을 물건은 PhysicsBody인 경우가 많습니다.
	FCollisionQueryParams Params;
	Params.AddIgnoredActor(this); // 나 자신은 무시

	TArray<FOverlapResult> OverlapResults;
	bool bHit = GetWorld()->OverlapMultiByChannel(
		OverlapResults,
		HandLocation,
		FQuat::Identity,
		ECC_PhysicsBody, // 잡을 물체의 콜리전 채널 (필요시 변경)
		Sphere,
		Params
	);

	// 3. 디버그용 원 그리기 (초록색: 성공, 빨간색: 실패)
	DrawDebugSphere(GetWorld(), HandLocation, GrabRadius, 12, bHit ? FColor::Green : FColor::Red, false, 1.0f);

	if (bHit)
	{
		// 4. 감지된 물체 중 '가장 가까운' + '인터페이스를 가진' 녀석 찾기
		AActor* ClosestActor = nullptr;
		float MinDistanceSq = FLT_MAX;

		for (const FOverlapResult& Result : OverlapResults)
		{
			AActor* HitActor = Result.GetActor();

			// 유효성 체크 & 인터페이스 구현 여부 체크
			if (HitActor && HitActor->Implements<UVRGrabInterface>())
			{
				float DistSq = FVector::DistSquared(HandLocation, HitActor->GetActorLocation());
				if (DistSq < MinDistanceSq)
				{
					MinDistanceSq = DistSq;
					ClosestActor = HitActor;
				}
			}
		}

		// 5. 대상이 있으면 Grab 실행
		if (ClosestActor)
		{
			// 인터페이스 호출: "나 너 잡았어" (Execute_ 접두사 필수!)
			IVRGrabInterface::Execute_Grab(ClosestActor, HandMesh);

			// 놓을 때를 대비해 변수에 저장
			OutHeldActor = ClosestActor;

			UE_LOG(LogTemp, Log, TEXT("Grabbed Actor: %s"), *ClosestActor->GetName());
		}
	}
}

void AVRCharacterPawn::TryReleaseActor(AActor*& InHeldActor, USceneComponent* HandMesh)
{
	if (InHeldActor)
	{
		// 인터페이스 호출: "나 너 놨어"
		if (InHeldActor->Implements<UVRGrabInterface>())
		{
			// [핵심] 손의 현재 물리 속도를 가져옵니다.
			FVector CurrentThrowVelocity = FVector::ZeroVector;
			if (HandMesh)
			{
				CurrentThrowVelocity = HandMesh->GetComponentVelocity();
			}
			IVRGrabInterface::Execute_Release(InHeldActor, CurrentThrowVelocity);
			UE_LOG(LogTemp, Log, TEXT("Released Actor w/ Velocity: %s"), *CurrentThrowVelocity.ToString());
		}

		// 변수 초기화
		InHeldActor = nullptr;
	}
}

void AVRCharacterPawn::OnGrabLeft(const FInputActionValue& Value)
{
	// 입력 값 (bool 또는 float) 가져오기
	float GripValue = Value.Get<float>();
	UE_LOG(LogTemp, Warning, TEXT("Left Grab Input Detected: %f"), GripValue);

	// 여기에 '물건 집기' 로직 호출
	// 버튼이 눌려있는가? (true: 누름, false: 뗌) -> GripValue가 0.5 이상이면 눌린 것으로 간주
	bool bIsPressed = GripValue > 0.5;

	if (bIsPressed)
	{
		TryGrabActor(LeftHandMesh, HeldActorLeft);
	}
	else
	{
		TryReleaseActor(HeldActorLeft, LeftHandMesh);
	}

}

void AVRCharacterPawn::OnGrabRight(const FInputActionValue& Value)
{
	float GripValue = Value.Get<float>();
	UE_LOG(LogTemp, Warning, TEXT("Right Grab Input Detected: %f"), GripValue);

	bool bIsPressed = GripValue > 0.5;

	//if (bIsPressed)
	//{
	//	TryGrabActor(RightHandMesh, HeldActorRight);
	//}
	//else
	//{
	//	TryReleaseActor(HeldActorRight, RightHandMesh);
	//}

	if (bIsPressed)
	{
		// 1. 이미 뭔가를 잡고 있다면? -> 놓는다 (기본 동작 유지 or 재장착)
		if (HeldActorRight)
		{
			// 놓을 때는 슬롯 상관없이 무조건 Release
			TryReleaseActor(HeldActorRight, RightHandMesh);
			return;
		}

		// 2. 빈손일 때 -> 슬롯에 따라 행동 결정
		switch (CurrentItemSlot)
		{
		case EItemSlot::None:
			// [빈손] 기존처럼 월드에 있는 물건 잡기 시도
			TryGrabActor(RightHandMesh, HeldActorRight);
			break;

		case EItemSlot::Tranquilizer:
			// [권총] 소환 후 강제 잡기
			if(bHasTranquilizer)
				SpawnAndEquip(TranquilizerClass, RightHandMesh, HeldActorRight);
			break;

		case EItemSlot::ThrownItem:
			// [머그컵] 소환 후 강제 잡기
			if(DistractionItemQuantity > 0) 
				SpawnAndEquip(ThrownItemClass, RightHandMesh, HeldActorRight);
			break;
		}
	}
	else
	{
		if (HeldActorRight)
		{
			// [수정] 엔진 함수 대신 우리가 직접 계산한 값 사용
			FVector FinalVelocity = CalculatedRightHandVelocity;

			// VR 투척 보정 (손맛을 위해 1.3배 ~ 1.5배 증폭)
			FinalVelocity *= 1.5f;

			UE_LOG(LogTemp, Warning, TEXT("THROW! Manual Velocity: %s (Speed: %f)"), *FinalVelocity.ToString(), FinalVelocity.Size());

			if (HeldActorRight->Implements<UVRGrabInterface>())
			{
				IVRGrabInterface::Execute_Release(HeldActorRight, FinalVelocity);
			}

			HeldActorRight = nullptr;

			// 버튼 뗄 때 -> 물건 놓기
			//TryReleaseActor(HeldActorRight, RightHandMesh);
		}
	}
}

void AVRCharacterPawn::Move(const FInputActionValue& Value)
{
	if (bIsDead) return;
	// 입력값 (X: 좌우, Y: 전후)
	FVector2D MovementVector = Value.Get<FVector2D>();

	if (Controller != nullptr)
	{
		// 카메라(헤드셋)가 바라보는 방향을 가져옴
		// *중요*: 카메라는 Pawn의 자식 컴포넌트이므로 CameraComponent를 참조해야 함
		// (만약 Camera 변수가 없다면 FindComponentByClass<UCameraComponent>() 사용)
		//UCameraComponent* Camera = FindComponentByClass<UCameraComponent>();
		if (!VRCamera) return;

		// A. 전후 이동 (Camera Forward)
		const FRotator Rotation = VRCamera->GetComponentRotation();
		const FRotator YawRotation(0, Rotation.Yaw, 0); // Z축(Pitch/Roll) 제거 -> 평면 이동

		// 평면화된 앞방향 벡터
		const FVector ForwardDirection = FRotationMatrix(YawRotation).GetUnitAxis(EAxis::X);
		AddMovementInput(ForwardDirection, MovementVector.Y);

		// B. 좌우 이동 (Camera Right)
		const FVector RightDirection = FRotationMatrix(YawRotation).GetUnitAxis(EAxis::Y);
		AddMovementInput(RightDirection, MovementVector.X);
	}
}

void AVRCharacterPawn::Turn(const FInputActionValue& Value)
{
	if (bIsDead) return;

	// 이제 기존 float 대신 FVector2D로 X(좌우), Y(상하) 축을 모두 가져옵니다.
	FVector2D TurnInput = Value.Get<FVector2D>();

	// 좌우 조작량 파악
	float TurnValue = TurnInput.X;

	// 데드존 처리 (조이스틱을 조금만 기울였을 때는 무시)
	if (FMath::Abs(TurnValue) < 0.5f) return;

	// [핵심] 조이스틱 허용 오차 (Tolerance) 설정: 45도 제한
	// Y값의 크기가 X값의 크기보다 크다는 것은, 상/하단으로 45도 이상 꺾였다는 뜻입니다.
	// 이 경우 플레이어가 턴을 의도한 것이 아니라 위아래 조작(다른 기능)을 의도한 것으로 보고 무시합니다.
	if (FMath::Abs(TurnInput.Y) > FMath::Abs(TurnInput.X))
	{
		return;
	}

	// 1. 혹시 모를 로컬 액터 강제 회전
	FRotator CurrentRot = GetActorRotation();
	CurrentRot.Yaw += (TurnValue > 0 ? SnapTurnAngle : -SnapTurnAngle);
	SetActorRotation(CurrentRot);

	// 2. 엔진 표준 컨트롤러 Yaw 회전
	AddControllerYawInput(TurnValue > 0 ? SnapTurnAngle : -SnapTurnAngle);
}

void AVRCharacterPawn::ResetTurn()
{
}

void AVRCharacterPawn::OnTriggerLeft(const FInputActionValue& Value)
{
	if (bIsDead) return;

	// 왼손에 잡은 물건이 있고 + 인터페이스를 지원한다면
	if (HeldActorLeft && HeldActorLeft->Implements<UVRGrabInterface>())
	{
		float Pressure = Value.Get<float>();
		IVRGrabInterface::Execute_OnAction(HeldActorLeft, Pressure);
	}
}

void AVRCharacterPawn::OnTriggerRight(const FInputActionValue& Value)
{
	//if (HeldActorRight && HeldActorRight->Implements<UVRGrabInterface>())
	//{
	//	float Pressure = Value.Get<float>();
	//	IVRGrabInterface::Execute_OnAction(HeldActorRight, Pressure);
	//}
	if (bIsDead) return;

	float InputValue = Value.Get<float>();

	// 1. 입력 확인: 트리거를 당겼을 때 이 로그가 뜨나요?
	// 안 뜬다면 -> Input Action 연결(IMC) 문제
	UE_LOG(LogTemp, Warning, TEXT("[1] Trigger Input Value: %f"), InputValue);

	if (HeldActorRight)
	{
		// 2. 변수 확인: 이 로그가 뜨나요?
		// 안 뜬다면 -> Grab 할 때 변수 저장이 실패한 것 (여기가 범인일 확률 90%)
		UE_LOG(LogTemp, Warning, TEXT("[2] Sending Signal to: %s"), *HeldActorRight->GetName());

		if (HeldActorRight->Implements<UVRGrabInterface>())
		{
			IVRGrabInterface::Execute_OnAction(HeldActorRight, InputValue);
		}
	}
	else
	{
		// 3. 변수가 비어있음
		UE_LOG(LogTemp, Error, TEXT("[3] HeldActorRight is NULL! (But gun is visually attached?)"));
	}

}

void AVRCharacterPawn::OnSprintStart(const FInputActionValue& Value)
{
	if (bIsDead) return;
	if (Value.Get<float>() > 0.5) {
		// 부모 클래스(HorrorCharacter)의 달리기 시작 함수 호출
		DoStartSprint();

		// 로그로 확인 (나중에 지우세요)
		// UE_LOG(LogTemp, Log, TEXT("Sprint STARTED"));
	}
}

void AVRCharacterPawn::OnSprintEnd(const FInputActionValue& Value)
{
	// 부모 클래스의 달리기 종료 함수 호출
	DoEndSprint();

	UE_LOG(LogTemp, Log, TEXT("Sprint ENDED"));
}

void AVRCharacterPawn::CycleItemNext(const FInputActionValue& Value)
{
	if (bIsDead) return;
	// 현재 값 + 1
	uint8 NextVal = (uint8)CurrentItemSlot + 1;
	if (NextVal >= (uint8)EItemSlot::Max)
	{
		NextVal = 0; // 다시 처음(None)으로
	}
	CurrentItemSlot = (EItemSlot)NextVal;

	PrintCurrentSlot();
}

void AVRCharacterPawn::CycleItemPrev(const FInputActionValue& Value)
{
	if (bIsDead) return;
	// 현재 값 - 1
	uint8 PrevVal = (uint8)CurrentItemSlot;
	if (PrevVal == 0)
	{
		PrevVal = (uint8)EItemSlot::Max - 1; // 끝으로 이동
	}
	else
	{
		PrevVal--;
	}
	CurrentItemSlot = (EItemSlot)PrevVal;

	PrintCurrentSlot();
}

void AVRCharacterPawn::PrintCurrentSlot()
{
	FString SlotName;
	switch (CurrentItemSlot)
	{
	case EItemSlot::None: SlotName = TEXT("EMPTY HAND (Interact)"); break;
	case EItemSlot::Tranquilizer: SlotName = TEXT("TRANQUILIZER (Spawn)"); break;
	case EItemSlot::ThrownItem: SlotName = TEXT("MUG (Spawn)"); break;
	}

	// 화면에 띄워서 확인
	GEngine->AddOnScreenDebugMessage(-1, 2.0f, FColor::Cyan, FString::Printf(TEXT("Slot: %s"), *SlotName));
} //flag for the file change


void AVRCharacterPawn::PlayDamageEffect()
{
	GetWorldTimerManager().ClearTimer(DamageEffectTimerHandle);

	CurrentDamageIntensity = 1.0f; // 효과 시작 시 최대값

	GetWorldTimerManager().SetTimer(DamageEffectTimerHandle, this, &AVRCharacterPawn::UpdateDamageEffect, 0.02f, true); // 약 30 FPS로 업데이트 = 0.033f

	// (선택 사항) 카메라 흔들림 추가! 
	// UGameplayStatics::PlayWorldCameraShake(...) 를 사용하면 더 역동적입니다.
}

void AVRCharacterPawn::UpdateDamageEffect()
{
	if (DamageMaterialInstance)
	{
		// 효과 감소
		CurrentDamageIntensity -= 0.02f; // 0.02씩 감소 -> 약 1초 지속
		if (CurrentDamageIntensity <= 0.0f)
		{
			CurrentDamageIntensity = 0.0f;
			GetWorldTimerManager().ClearTimer(DamageEffectTimerHandle); // 타이머 종료
		}
		// 머티리얼에 현재 강도 전달
		DamageMaterialInstance->SetScalarParameterValue(TEXT("Intensity"), CurrentDamageIntensity);
	}
}


