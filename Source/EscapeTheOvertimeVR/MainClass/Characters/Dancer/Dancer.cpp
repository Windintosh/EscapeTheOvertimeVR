#include "MainClass/Characters/Dancer/Dancer.h"
#include "NavModifierComponent.h"       // NavModifier 컴포넌트 헤더
#include "NavAreas/NavArea_Obstacle.h"
#include "GameFramework/CharacterMovementComponent.h"

// Sets default values
ADancer::ADancer()
{
 	// Set this character to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

    // 1. NavModifier 컴포넌트 생성
    NavModifierComp = CreateDefaultSubobject<UNavModifierComponent>(TEXT("NavModifierComp"));

    // 2. 영역 클래스(Area Class) 설정
    // UNavArea_Obstacle은 기본적으로 높은 비용을 가집니다.
    // 다른 AI는 이 영역을 피해 경로를 잡으려 노력합니다.
    NavModifierComp->SetAreaClass(UNavArea_Obstacle::StaticClass());

    // 3. 범위 설정 (선택 사항)
    // 캐릭터의 캡슐 크기에 맞춰 범위를 조정합니다.
    // FVector(X, Y, Z) 크기입니다. 적절한 크기로 조절하세요.
    NavModifierExtentSize = FVector(50.f, 50.f, 100.f);

    NavModifierComp->FailsafeExtent = NavModifierExtentSize;

    UCharacterMovementComponent* MovementComponent = GetCharacterMovement();
    if (MovementComponent)
    {
        MovementComponent->bUseRVOAvoidance = true;
        MovementComponent->AvoidanceConsiderationRadius = 200.f;
        MovementComponent->AvoidanceWeight = 0.5f;
        UE_LOG(LogTemp, Display, TEXT("RVO is enabled"));
    }
}

// Called when the game starts or when spawned
void ADancer::BeginPlay()
{
	Super::BeginPlay();
	
}

// Called every frame
void ADancer::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

// Called to bind functionality to input
void ADancer::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);

}

