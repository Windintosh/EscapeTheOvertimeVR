#include "MainClass/Objects/WorldObjects/PatternLock.h" // [수정] 경로 변경: Puzzles -> WorldObjects
#include "Components/StaticMeshComponent.h"
#include "Components/BoxComponent.h"
#include "Kismet/GameplayStatics.h"
#include "Sound/SoundBase.h"

// 생성자: 컴포넌트 초기화
APatternLock::APatternLock()
{
	PrimaryActorTick.bCanEverTick = false;

	// 루트 컴포넌트
	RootScene = CreateDefaultSubobject<USceneComponent>(TEXT("RootScene"));
	SetRootComponent(RootScene);

	// 베이스 메쉬
	BaseMesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("BaseMesh"));
	BaseMesh->SetupAttachment(RootScene);

	// 슬롯 메쉬들 초기화 및 태그 설정
	SlotMesh_01 = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("SlotMesh_01"));
	SlotMesh_01->SetupAttachment(BaseMesh);
	SlotMesh_01->ComponentTags.Add(FName("Slot0"));

	SlotMesh_02 = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("SlotMesh_02"));
	SlotMesh_02->SetupAttachment(BaseMesh);
	SlotMesh_02->ComponentTags.Add(FName("Slot1"));

	SlotMesh_03 = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("SlotMesh_03"));
	SlotMesh_03->SetupAttachment(BaseMesh);
	SlotMesh_03->ComponentTags.Add(FName("Slot2"));

	// 콜리전 박스 (전체 영역 감지용)
	TouchCollision = CreateDefaultSubobject<UBoxComponent>(TEXT("TouchCollision"));
	TouchCollision->SetupAttachment(BaseMesh);
	TouchCollision->SetBoxExtent(FVector(20.f, 10.f, 10.f));

	// 기본값 초기화
	bIsSolved = false;

	// 현재 코드 0,0,0 초기화 (크기 3)
	CurrentCode.Init(0, 3);

	// 타겟 코드 기본값 (에디터에서 수정 가능)
	TargetCode.Init(0, 3);
}

void APatternLock::BeginPlay()
{
	Super::BeginPlay();

	// 게임 시작 시 초기 상태 비주얼 업데이트 (모두 0번 문양으로)
	UpdateSlotVisual(0);
	UpdateSlotVisual(1);
	UpdateSlotVisual(2);
}

// 메인 상호작용 함수: 슬롯을 누르면 호출됨
void APatternLock::InteractSlot(int32 SlotIndex)
{
	// 이미 풀렸거나 인덱스가 잘못됐으면 무시
	if (bIsSolved || !CurrentCode.IsValidIndex(SlotIndex))
	{
		return;
	}

	// 1. 값 증가 및 순환 (Modulo 연산)
	// 머티리얼 배열의 크기(5)로 나눈 나머지를 저장하여 0~4 순환
	int32 MaxSymbols = SymbolMaterials.Num();
	if (MaxSymbols > 0)
	{
		CurrentCode[SlotIndex] = (CurrentCode[SlotIndex] + 1) % MaxSymbols;
	}

	// 2. 효과음 재생 (딸깍)
	if (ClickSound)
	{
		UGameplayStatics::PlaySoundAtLocation(this, ClickSound, GetActorLocation());
	}

	// 3. 시각화 업데이트
	UpdateSlotVisual(SlotIndex);

	// 4. 정답 확인
	CheckAnswer();
}

// 슬롯의 머티리얼을 현재 코드 값에 맞춰 변경
void APatternLock::UpdateSlotVisual(int32 SlotIndex)
{
	if (!CurrentCode.IsValidIndex(SlotIndex)) return;

	int32 SymbolIndex = CurrentCode[SlotIndex];

	// 머티리얼 배열이 비어있거나 인덱스가 벗어나면 리턴
	if (!SymbolMaterials.IsValidIndex(SymbolIndex)) return;

	// 변경할 메쉬 찾기
	UStaticMeshComponent* TargetMesh = GetMeshBySlotIndex(SlotIndex);
	if (TargetMesh)
	{
		// 머티리얼 교체 (Element 0번 교체 가정)
		TargetMesh->SetMaterial(0, SymbolMaterials[SymbolIndex]);
	}
}

// 정답 비교 로직
void APatternLock::CheckAnswer()
{
	// 타겟 코드 설정이 안되어 있으면 리턴
	if (TargetCode.Num() != 3) return;

	// 1. 코드 비교
	bool bMatch = true;
	for (int32 i = 0; i < 3; i++)
	{
		if (CurrentCode[i] != TargetCode[i])
		{
			bMatch = false;
			break;
		}
	}

	// 2. 정답 처리
	if (bMatch)
	{
		bIsSolved = true;
		UE_LOG(LogTemp, Warning, TEXT("Pattern Lock Solved!"));

		// 성공 사운드
		if (SuccessSound)
		{
			UGameplayStatics::PlaySoundAtLocation(this, SuccessSound, GetActorLocation());
		}

		// 델리게이트 방송 -> 문(Door) BP에서 이 신호를 받아서 열림
		if (OnUnlocked.IsBound())
		{
			OnUnlocked.Broadcast();
		}
	}
}

// 슬롯 인덱스에 따라 컴포넌트 포인터 반환 (헬퍼 함수)
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