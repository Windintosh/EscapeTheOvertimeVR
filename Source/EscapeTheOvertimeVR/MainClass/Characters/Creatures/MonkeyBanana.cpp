#include "MainClass/Characters/Creatures/MonkeyBanana.h"

// Sets default values
AMonkeyBanana::AMonkeyBanana()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	// 발사체 설정
	ProjectileMovement = CreateDefaultSubobject<UProjectileMovementComponent>(TEXT("ProjectileMovementComp"));
	ProjectileMovement->UpdatedComponent = RootComponent;
	ProjectileMovement->InitialSpeed = 1500.f; // 속도 조금 더 올림
	ProjectileMovement->MaxSpeed = 2000.f;
	ProjectileMovement->bRotationFollowsVelocity = true;
	ProjectileMovement->bSweepCollision = true;
	ProjectileMovement->bSimulationEnabled = true; // 이동 시뮬레이션 켜기
	ProjectileMovement->bInterpMovement = true;
	ProjectileMovement->bForceSubStepping = true; // 정확도 향상
	ProjectileMovement->bShouldBounce = false; // 튕기기 끔 -> 즉시 Stop 이벤트 발생
	ProjectileMovement->ProjectileGravityScale = 1.0f;

	RotatingMovement = CreateDefaultSubobject<URotatingMovementComponent>(TEXT("RotatingMovementComp"));
	RotatingMovement->RotationRate = FRotator(0.f, 0.f, 360.f); // 초당 360도 회전
}

// Called when the game starts or when spawned
void AMonkeyBanana::BeginPlay()
{
	Super::BeginPlay();
	
}

// Called every frame
void AMonkeyBanana::Tick(float DeltaTime)
{
	//Super::Tick(DeltaTime);

}

