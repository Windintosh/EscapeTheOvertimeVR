#include "MainClass/Characters/VRCharacterPawn/VRCharacterPawn.h"

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
		}

		if (GrabRightAction)
		{
			EnhancedInputComponent->BindAction(GrabRightAction, ETriggerEvent::Started, this, &AVRCharacterPawn::OnGrabRight);
		}
	}
}

void AVRCharacterPawn::OnGrabLeft(const FInputActionValue& Value)
{
	// 입력 값 (bool 또는 float) 가져오기
	float GripValue = Value.Get<float>();
	UE_LOG(LogTemp, Warning, TEXT("Left Grab Input Detected: %f"), GripValue);

	// 여기에 '물건 집기' 로직 호출
}

void AVRCharacterPawn::OnGrabRight(const FInputActionValue& Value)
{
	UE_LOG(LogTemp, Warning, TEXT("Right Grab Input Detected"));
}


