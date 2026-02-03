#include "MainClass/Objects/Puzzle/LockSystems/PatternLock.h"
#include "Components/StaticMeshComponent.h"
#include "Kismet/GameplayStatics.h"
#include "Sound/SoundBase.h"

APatternLock::APatternLock()
{
	PrimaryActorTick.bCanEverTick = false;

	RootScene = CreateDefaultSubobject<USceneComponent>(TEXT("RootScene"));
	SetRootComponent(RootScene);

	BaseMesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("BaseMesh"));
	BaseMesh->SetupAttachment(RootScene);

	// 버튼 슬롯 초기화
	SlotMesh_01 = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("SlotMesh_01"));
	SlotMesh_01->SetupAttachment(BaseMesh);
	SlotMesh_01->ComponentTags.Add(FName("Slot0"));

	SlotMesh_02 = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("SlotMesh_02"));
	SlotMesh_02->SetupAttachment(BaseMesh);
	SlotMesh_02->ComponentTags.Add(FName("Slot1"));

	SlotMesh_03 = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("SlotMesh_03"));
	SlotMesh_03->SetupAttachment(BaseMesh);
	SlotMesh_03->ComponentTags.Add(FName("Slot2"));

	// 변수 초기화
	bIsSolved = false;
	CurrentCode.Init(0, 3);
	TargetCode.Init(0, 3);
}

void APatternLock::BeginPlay()
{
	Super::BeginPlay();

	// 초기 시각화 업데이트
	UpdateSlotVisual(0);
	UpdateSlotVisual(1);
	UpdateSlotVisual(2);

	// 정답 랜덤 생성
	RandomizeTargetCode();
}

void APatternLock::RandomizeTargetCode()
{
	if (SymbolMaterials.Num() == 0) return;

	for (int32 i = 0; i < 3; i++)
	{
		TargetCode[i] = FMath::RandRange(0, SymbolMaterials.Num() - 1);
	}

	// 디버깅용 로그 (패키징 시에는 주석 처리 권장)
	UE_LOG(LogTemp, Warning, TEXT("PatternLock Randomized Target: %d %d %d"), TargetCode[0], TargetCode[1], TargetCode[2]);
}

void APatternLock::InteractSlot(int32 SlotIndex)
{
	// 이미 풀렸거나 유효하지 않은 슬롯이면 무시
	if (bIsSolved || !CurrentCode.IsValidIndex(SlotIndex)) return;

	int32 MaxSymbols = SymbolMaterials.Num();
	if (MaxSymbols > 0)
	{
		// 순환 로직 (0 -> 1 -> ... -> 4 -> 0)
		CurrentCode[SlotIndex] = (CurrentCode[SlotIndex] + 1) % MaxSymbols;
	}

	if (ClickSound)
	{
		UGameplayStatics::PlaySoundAtLocation(this, ClickSound, GetActorLocation());
	}

	UpdateSlotVisual(SlotIndex);
	CheckAnswer();
}

void APatternLock::UpdateSlotVisual(int32 SlotIndex)
{
	if (!CurrentCode.IsValidIndex(SlotIndex)) return;
	int32 SymbolIndex = CurrentCode[SlotIndex];

	// 머티리얼 배열 유효성 검사
	if (!SymbolMaterials.IsValidIndex(SymbolIndex)) return;

	UStaticMeshComponent* TargetMesh = GetMeshBySlotIndex(SlotIndex);
	if (TargetMesh)
	{
		TargetMesh->SetMaterial(0, SymbolMaterials[SymbolIndex]);
	}
}

void APatternLock::CheckAnswer()
{
	if (TargetCode.Num() != 3) return;

	bool bMatch = true;
	for (int32 i = 0; i < 3; i++)
	{
		if (CurrentCode[i] != TargetCode[i])
		{
			bMatch = false;
			break;
		}
	}

	if (bMatch)
	{
		bIsSolved = true;
		UE_LOG(LogTemp, Warning, TEXT("Pattern Lock Solved!"));

		if (SuccessSound)
		{
			UGameplayStatics::PlaySoundAtLocation(this, SuccessSound, GetActorLocation());
		}

		if (OnUnlocked.IsBound())
		{
			OnUnlocked.Broadcast();
		}
	}
}

UStaticMeshComponent* APatternLock::GetMeshBySlotIndex(int32 SlotIndex) const
{
	switch (SlotIndex)
	{
	case 0: return SlotMesh_01;
	case 1: return SlotMesh_02;
	case 2: return SlotMesh_03;
	default: return nullptr;
	}
}