// Fill out your copyright notice in the Description page of Project Settings.

#include "LocationTrigger.h"
#include "Variant_Horror/HorrorCharacter.h" // 플레이어 캐스팅을 위해 포함

// Sets default values
ALocationTrigger::ALocationTrigger()
{
	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	// 트리거 박스 생성 및 루트 설정
	TriggerBox = CreateDefaultSubobject<UBoxComponent>(TEXT("TriggerBox"));
	RootComponent = TriggerBox;

	// 충돌 프로필 설정 (OverlapAllDynamic 등, 트리거용 설정)
	TriggerBox->SetCollisionProfileName(TEXT("Trigger"));

	// 박스 크기 기본값 설정 (에디터에서 조절 가능)
	TriggerBox->InitBoxExtent(FVector(200.0f, 200.0f, 100.0f));

	// 오버랩 이벤트 바인딩
	TriggerBox->OnComponentBeginOverlap.AddDynamic(this, &ALocationTrigger::OnOverlapBegin);
}

// Called when the game starts or when spawned
void ALocationTrigger::BeginPlay()
{
	Super::BeginPlay();

}

// Called every frame
void ALocationTrigger::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

// 오버랩 발생 시 로직 구현
void ALocationTrigger::OnOverlapBegin(UPrimitiveComponent* OverlappedComp, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{
	// 1. 들어온 액터가 존재하는지, 그리고 플레이어(HorrorCharacter)인지 확인
	if (OtherActor && (OtherActor != this))
	{
		AHorrorCharacter* PlayerCharacter = Cast<AHorrorCharacter>(OtherActor);

		if (PlayerCharacter)
		{
			// 2. 플레이어의 현재 장소 정보(CurrentDeathLocation)를 이 트리거의 설정값으로 갱신
			PlayerCharacter->SetDeathLocation(LocationType);

			// (디버깅용 로그 - 필요 시 주석 해제)
			UE_LOG(LogTemp, Log, TEXT("Player entered location: %d"), (int32)LocationType);
		}
	}
}
