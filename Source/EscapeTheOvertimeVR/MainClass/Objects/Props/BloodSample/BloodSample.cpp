#include "MainClass/Objects/Props/BloodSample/BloodSample.h"
#include "Components/StaticMeshComponent.h"
#include "Kismet/KismetMathLibrary.h"

ABloodSample::ABloodSample()
{
	// PropBase에서 설정한 Tick 설정을 유지
	PrimaryActorTick.bCanEverTick = true;

	// 1. 혈액 액체 메쉬 설정
	// 부모인 PropBase의 StaticMesh(유리관)에 부착합니다.
	BloodLiquidMesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("BloodLiquid"));
	BloodLiquidMesh->SetupAttachment(StaticMesh);
	BloodLiquidMesh->SetCollisionEnabled(ECollisionEnabled::NoCollision);

	// 2. 눈알 메쉬 설정
	// 액체 메쉬의 자식으로 설정하여 상대 위치를 관리하기 쉽게 합니다.
	EyeballMesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("Eyeball"));
	EyeballMesh->SetupAttachment(BloodLiquidMesh);
	EyeballMesh->SetCollisionEnabled(ECollisionEnabled::NoCollision);
}

void ABloodSample::BeginPlay()
{
	Super::BeginPlay(); // 부모의 BeginPlay 호출 (Overlap 이벤트 바인딩 등 수행)

	if (EyeballMesh)
	{
		InitialEyeLocation = EyeballMesh->GetRelativeLocation();
	}
}

void ABloodSample::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime); // 부모의 Tick 호출

	if (!EyeballMesh) return;

	// --- 로직 1: 부력 연출 (항상 월드 좌표 기준 위(하늘)를 바라봄) ---
	// 실린더를 뒤집어도 눈동자가 항상 위를 향하도록 월드 회전값을 고정/보간합니다.
	FRotator CurrentWorldRot = EyeballMesh->GetComponentRotation();

	// 목표 회전값: 월드 기준 0,0,0 (눈알 모델의 기본 방향이 위인 경우)
	// 모델에 따라 FRotator(90, 0, 0) 등으로 조정이 필요할 수 있습니다.
	FRotator TargetWorldRot = FRotator::ZeroRotator;

	FRotator SmoothedRot = FMath::RInterpTo(CurrentWorldRot, TargetWorldRot, DeltaTime, EyeRotationSpeed);
	EyeballMesh->SetWorldRotation(SmoothedRot);

	// --- 로직 2: 액체 내 부유 연출 (Sine 곡선 활용) ---
	RunningTime += DeltaTime;
	float VerticalOffset = FMath::Sin(RunningTime * FloatFrequency) * FloatAmplitude;

	FVector NewRelativeLoc = InitialEyeLocation;
	NewRelativeLoc.Z += VerticalOffset;

	EyeballMesh->SetRelativeLocation(NewRelativeLoc);
}