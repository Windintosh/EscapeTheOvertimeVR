#include "MainClass/Characters/Creatures/MonkeyBanana.h"
#include "VRCharacterPawn.h"

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

	Damage = 10.f;
	KnockbackStrength = 1000.f;
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

void AMonkeyBanana::OnPlayerOverlap(UPrimitiveComponent* OverlappedComp, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{
	Super::OnPlayerOverlap(OverlappedComp, OtherActor, OtherComp, OtherBodyIndex, bFromSweep, SweepResult);

	//OtherComp->AddImpulse(ProjectileMovement->Velocity * 0.5f, NAME_None, true); // 충돌 시 플레이어에게 반동 추가
	/*
	// 부딪힌 대상이 캐릭터(플레이어)인지 확인합니다.
	AVRCharacterPawn* HitPlayer = Cast<AVRCharacterPawn>(OtherActor);

	if (HitPlayer)
	{
		// 1. 밀려날 방향 계산 (바나나에서 플레이어를 향하는 방향)
		FVector KnockbackDir = (HitPlayer->GetActorLocation() - GetActorLocation()).GetSafeNormal();

		// 2. [핵심] 위로 살짝 뜨게 만들기
		// 단순히 뒤로만 밀면 바닥 마찰력 때문에 캐릭터가 질질 끌리다 멈춥니다.
		// Z축 값을 살짝 올려서 공중에 띄워야 시원하게 밀려납니다!
		KnockbackDir.Z = 0.5f;
		KnockbackDir.Normalize();

		//float CurrentVelo = ProjectileMovement->Velocity.Size();

		// 3. 밀쳐낼 힘(속도) 설정
		//KnockbackStrength = ProjectileMovement->Velocity.Size() * 1.1f; // 이 값을 조절해서 밀쳐지는 거리를 맞추세요.
		FVector LaunchVelocity = KnockbackDir * KnockbackStrength;

		// 4. 캐릭터 발사!
		// 파라미터 2, 3번(bXYOverride, bZOverride)을 true로 하면 기존에 움직이던 관성을 무시하고 확 밀쳐냅니다.
		HitPlayer->LaunchCharacter(LaunchVelocity, true, true);

		// (선택) 바나나 파괴
		Destroy();
	}
	*/
}

