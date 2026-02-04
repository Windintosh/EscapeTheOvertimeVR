#include "WarpBox.h"
#include "Components/BoxComponent.h"
#include "HorrorCharacter.h"
#include "Kismet/GameplayStatics.h"
#include "TimerManager.h"
#include "ETOGameState.h"
#include "ElevatorDoor.h"

AWarpBox::AWarpBox()
{
	PrimaryActorTick.bCanEverTick = false;

	TriggerBox = CreateDefaultSubobject<UBoxComponent>(TEXT("TriggerBox"));
	RootComponent = TriggerBox;

	// 트리거 설정
	TriggerBox->SetCollisionProfileName(TEXT("Trigger"));
	TriggerBox->SetBoxExtent(FVector(100.f, 100.f, 100.f));
}

void AWarpBox::BeginPlay()
{
	Super::BeginPlay();

	// 오버랩 이벤트 바인딩
	TriggerBox->OnComponentBeginOverlap.AddDynamic(this, &AWarpBox::OnOverlapBegin);
}

void AWarpBox::OnOverlapBegin(UPrimitiveComponent* OverlappedComp, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{
	// 이미 타이머가 돌고 있다면 무시
	if (GetWorld()->GetTimerManager().IsTimerActive(WarpTimerHandle))
	{
		return;
	}

	AHorrorCharacter* Player = Cast<AHorrorCharacter>(OtherActor);
	if (Player)
	{
		Player->bIsGameCleared = true;

		AETOGameState* GameState = Cast<AETOGameState>(GetWorld()->GetGameState());
		GameState->SavePlayerHP();

		TArray<AActor*> EVDoors;
		UGameplayStatics::GetAllActorsOfClass(GetWorld(), AElevatorDoor::StaticClass(), EVDoors);

		for (AActor* Door : EVDoors)
		{
			AElevatorDoor* EVDoor = Cast<AElevatorDoor>(Door);

			EVDoor->CloseDoorOnTimer();
		}


		UE_LOG(LogTemp, Warning, TEXT("Player entered WarpBox. Teleporting in %.1f seconds..."), WarpDelayTime);

		// 4초 뒤 WarpLevel 함수 실행
		GetWorld()->GetTimerManager().SetTimer(WarpTimerHandle, this, &AWarpBox::WarpLevel, WarpDelayTime, false);
	}
}

void AWarpBox::WarpLevel()
{
	// 1. 현재 레벨 이름 가져오기 (true: UEDPIE 같은 접두사 제거하고 순수 이름만 가져옴)
	FString CurrentLevelName = UGameplayStatics::GetCurrentLevelName(this, true);
	FName LevelToLoad = NAME_None;

	// 2. 현재 맵 이름에 따른 분기 처리
	if (CurrentLevelName == "FirstMap")
	{
		LevelToLoad = FName("TuesdayMap");
	}
	else if (CurrentLevelName == "TuesdayMap")
	{
		LevelToLoad = FName("WednesdayMap");
	}
	else if (CurrentLevelName == "WednesdayMap")
	{
		LevelToLoad = FName("ThursdayMap");
	}
	else if (CurrentLevelName == "ThursdayMap")
	{
		// 금요일 맵이 있다면 추가 (예시)
		LevelToLoad = FName("FridayMap");
	}
	// [추가] 금요일 맵(FridayMap)일 경우 엔딩 로직 실행
	else if (CurrentLevelName == "FridayMap")
	{
		AHorrorCharacter* PlayerCharacter = Cast<AHorrorCharacter>(GetWorld()->GetFirstPlayerController()->GetCharacter());
		AETOGameState* GameState = Cast<AETOGameState>(GetWorld()->GetGameState());

		PlayerCharacter->SetHP(100.f);
		GameState->SaveStuffs();

		UE_LOG(LogTemp, Warning, TEXT("Friday Map Cleared! Triggering Ending Sequence..."));

		// 델리게이트 방송 -> 레벨 블루프린트가 듣고 시네마틱 재생
		if (OnEndingTriggered.IsBound())
		{
			OnEndingTriggered.Broadcast();
		}

		// 레벨 이동을 하지 않고 여기서 함수 종료
		return;
	}

	// 3. 레벨 이동 실행
	if (!LevelToLoad.IsNone())
	{
		UE_LOG(LogTemp, Warning, TEXT("Current Level: %s -> Loading Next Level: %s"), *CurrentLevelName, *LevelToLoad.ToString());
		UGameplayStatics::OpenLevel(this, LevelToLoad);
	}
	else
	{
		UE_LOG(LogTemp, Error, TEXT("No next level defined for current map: %s"), *CurrentLevelName);
	}
}
