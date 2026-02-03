#include "MainClass/Objects/Puzzle/Hint_Monitor.h"
#include "MainClass/Objects/Puzzle/LockSystems/PatternLock.h"
#include "Components/StaticMeshComponent.h"
#include "Components/BoxComponent.h"
#include "Kismet/GameplayStatics.h"
#include "Sound/SoundBase.h"

AHint_Monitor::AHint_Monitor()
{
	PrimaryActorTick.bCanEverTick = false;

	RootScene = CreateDefaultSubobject<USceneComponent>(TEXT("RootScene"));
	SetRootComponent(RootScene);

	// 본체 (데스크탑)
	BodyMesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("BodyMesh"));
	BodyMesh->SetupAttachment(RootScene);

	// 상호작용 트리거 (본체 근처 감지)
	InteractTrigger = CreateDefaultSubobject<UBoxComponent>(TEXT("InteractTrigger"));
	InteractTrigger->SetupAttachment(BodyMesh);
	InteractTrigger->SetBoxExtent(FVector(30.f, 30.f, 30.f));
	InteractTrigger->SetCollisionProfileName(TEXT("Trigger")); // Overlap 전용

	// 모니터 3개
	MonitorMesh_01 = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("MonitorMesh_01"));
	MonitorMesh_01->SetupAttachment(BodyMesh);

	MonitorMesh_02 = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("MonitorMesh_02"));
	MonitorMesh_02->SetupAttachment(BodyMesh);

	MonitorMesh_03 = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("MonitorMesh_03"));
	MonitorMesh_03->SetupAttachment(BodyMesh);

	// 기본값 초기화
	bIsHintOn = false;
	TargetLock = nullptr;
	MonitorScreenMaterialIndex = 1; // 보통 0번은 프레임, 1번이 화면인 경우가 많음
}

void AHint_Monitor::BeginPlay()
{
	Super::BeginPlay();

	// 게임 시작 시 꺼진 화면으로 초기화
	UpdateMonitorVisual();
}

void AHint_Monitor::ToggleHint()
{
	// 연결된 패드가 없으면 에러 로그 출력
	if (!TargetLock)
	{
		UE_LOG(LogTemp, Error, TEXT("Hint_Monitor: No TargetLock assigned! Please check level."));
		return;
	}

	// 상태 토글 (ON <-> OFF)
	bIsHintOn = !bIsHintOn;

	// 효과음 재생 (켜짐/꺼짐)
	if (ToggleSound)
	{
		UGameplayStatics::PlaySoundAtLocation(this, ToggleSound, GetActorLocation());
	}

	// 화면 갱신
	UpdateMonitorVisual();
}

void AHint_Monitor::UpdateMonitorVisual()
{
	// 모니터 배열 관리
	UStaticMeshComponent* Monitors[3] = { MonitorMesh_01, MonitorMesh_02, MonitorMesh_03 };

	for (int32 i = 0; i < 3; i++)
	{
		if (!Monitors[i]) continue;

		if (bIsHintOn && TargetLock)
		{
			// 1. Lock에서 정답 코드와 심볼 목록 가져오기
			TArray<int32> AnswerCode = TargetLock->GetTargetCode();
			TArray<UMaterialInterface*> Symbols = TargetLock->GetSymbolMaterials();

			if (AnswerCode.IsValidIndex(i) && Symbols.IsValidIndex(AnswerCode[i]))
			{
				// 지정된 인덱스(화면)의 머티리얼만 교체
				Monitors[i]->SetMaterial(MonitorScreenMaterialIndex, Symbols[AnswerCode[i]]);
			}
		}
		else
		{
			// 꺼짐 상태 (검은 화면)
			if (MonitorOffMaterial)
			{
				Monitors[i]->SetMaterial(MonitorScreenMaterialIndex, MonitorOffMaterial);
			}
		}
	}
}