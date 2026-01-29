#include "MainClass/Characters/VRCharacterPawn/VRCharacterPawn.h"
#include "Engine/OverlapResult.h" // 충돌 결과 처리용
#include "DrawDebugHelpers.h"     // 디버그 원 그리기용

// Sets default values
AVRCharacterPawn::AVRCharacterPawn()
{
 	// Set this pawn to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	// 1. 루트 컴포넌트 생성 (기준점)
	VROrigin = CreateDefaultSubobject<USceneComponent>(TEXT("VROrigin"));
	RootComponent = VROrigin;

	// 2. 카메라 생성 및 부착
	VRCamera = CreateDefaultSubobject<UCameraComponent>(TEXT("VRCamera"));
	VRCamera->SetupAttachment(VROrigin);

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
	}
}

// Called every frame
void AVRCharacterPawn::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

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
			EnhancedInputComponent->BindAction(GrabLeftAction, ETriggerEvent::Started, this, &AVRCharacterPawn::OnGrabLeft);
			EnhancedInputComponent->BindAction(GrabLeftAction, ETriggerEvent::Completed, this, &AVRCharacterPawn::OnGrabLeft); // 같은 함수에서 bool 값으로 분기하거나 함수를 나누거나 선택->나중에 함수 나눌 예정.
		}

		if (GrabRightAction)
		{
			EnhancedInputComponent->BindAction(GrabRightAction, ETriggerEvent::Started, this, &AVRCharacterPawn::OnGrabRight);
			EnhancedInputComponent->BindAction(GrabRightAction, ETriggerEvent::Completed, this, &AVRCharacterPawn::OnGrabRight);
		}
	}
}

void AVRCharacterPawn::TryGrabActor(USceneComponent* HandMesh, AActor*& OutHeldActor)
{
	// 1. 이미 뭔가를 잡고 있다면 무시
	if (OutHeldActor) return;

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

	if (bIsPressed)
	{
		TryGrabActor(RightHandMesh, HeldActorRight);
	}
	else
	{
		TryReleaseActor(HeldActorRight, RightHandMesh);
	}
}


