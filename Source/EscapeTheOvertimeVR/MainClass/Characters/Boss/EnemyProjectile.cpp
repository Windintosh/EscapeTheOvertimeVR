#include "MainClass/Characters/Boss/EnemyProjectile.h"
#include "Variant_Horror/HorrorCharacter.h"
#include "VRCharacterPawn.h"
#include "Kismet/GameplayStatics.h"

// Sets default values
AEnemyProjectile::AEnemyProjectile()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	//Scene = CreateDefaultSubobject<USceneComponent>(TEXT("Scene"));
	//SetRootComponent(Scene);

	Collision = CreateDefaultSubobject<UBoxComponent>(TEXT("Collision"));
	Collision->SetCollisionProfileName(TEXT("OverlapAllDynamic"));
	Collision->SetCollisionEnabled(ECollisionEnabled::QueryOnly);
	Collision->SetCollisionObjectType(ECC_WorldDynamic);
	Collision->SetCollisionResponseToAllChannels(ECR_Overlap);
	Collision->SetCollisionResponseToChannel(ECC_Pawn, ECR_Overlap);
	Collision->SetCollisionResponseToChannel(ECC_WorldStatic, ECR_Overlap);
	Collision->SetCollisionResponseToChannel(ECC_WorldDynamic, ECR_Overlap);
	Collision->SetGenerateOverlapEvents(true);
	Collision->InitBoxExtent(FVector(40.0f, 40.0f, 40.0f));
	SetRootComponent(Collision);
	//Collision->SetupAttachment(Scene);

	StaticMesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("StaticMesh"));
	StaticMesh->SetupAttachment(Collision);
	StaticMesh->SetCollisionProfileName(TEXT("NoCollision"));
	
	
}

// Called when the game starts or when spawned
void AEnemyProjectile::BeginPlay()
{
	Super::BeginPlay();

	Collision->OnComponentBeginOverlap.AddDynamic(this, &AEnemyProjectile::OnPlayerOverlap);

	// 플레이어를 향해 날아가는 모드일 때만 방향을 계산합니다.
	if (bShootTowardsPlayer)
	{
		FireAtPlayer();
	}
	
}

void AEnemyProjectile::OnPlayerOverlap(UPrimitiveComponent* OverlappedComp, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{
	UE_LOG(LogTemp, Error, TEXT("Projectile: Overlapped Object %s"), *OtherActor->GetName());
	if (!OtherActor || OtherActor == this || OtherActor == GetOwner())
	{
		UE_LOG(LogTemp, Error, TEXT("Projectile: Overlapped Object is not a Player!"));
		return;
	}
	
	AHorrorCharacter* PlayerCharacter = Cast<AHorrorCharacter>(OtherActor);
	if (PlayerCharacter)
	{
		// 플레이어라면 대미지를 주고
		//PlayerCharacter->DamagePlayer(40.f);
		AController* OwnerController = nullptr;
		if (GetOwner())
		{
			OwnerController = GetOwner()->GetInstigatorController();
		}

		// 3. 엔진 표준 대미지 전달 함수 호출
		UGameplayStatics::ApplyDamage(
			OtherActor,                     // 맞는 사람
			Damage,                          // 대미지 양
			OwnerController,                // 때린 사람의 컨트롤러 (Instigator)
			this,                           // 대미지를 가한 도구 (Projectile)
			UDamageType::StaticClass()      // 대미지 타입 (기본형)
		);

		KnockbackPlayer(PlayerCharacter);
		
		// 발사체 파괴
		DestroyProjectile();
	}
	else
	{
		// 3. 플레이어가 아니지만 충돌한 경우 (벽, 바닥 등)
		DestroyProjectile();
	}
}

void AEnemyProjectile::DestroyProjectile()
{
	UE_LOG(LogTemp, Display, TEXT("%s is destroyed."), *GetName());
	Destroy();
}

// Called every frame
void AEnemyProjectile::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	/*
	// 1. 이번 프레임에 이동할 거리 계산 (방향 * 속도 * 시간)
	FVector DeltaLocation = ShootDirection * Speed * DeltaTime;

	// 2. 투사체 이동 적용
	// bSweep 매개변수를 true로 설정하는 것이 아주 중요해!
	// 그래야 이동하는 도중에 플레이어나 벽을 뚫고 지나가지 않고 Hit/Overlap 판정이 발생하거든.
	AddActorWorldOffset(DeltaLocation, true);
	*/
	/*
	FVector NewLocation = GetActorLocation() + GetActorForwardVector() * Speed * DeltaTime;

	FHitResult Hit;

	SetActorLocation(NewLocation, true, &Hit); //bSweep = true to check collision

	Collision->UpdateOverlaps();

	if (Hit.IsValidBlockingHit())
	{
		DestroyProjectile();
	}

	if (Collision)
	{
		DrawDebugSphere(GetWorld(), Collision->GetComponentLocation(), Collision->GetScaledSphereRadius(), 12, FColor::Red, false, -1.0f);
	}
	*/

	// 1. bool 변수에 따라 이동할 '방향(Direction)'을 먼저 결정합니다.
	FVector MoveDirection;

	if (bShootTowardsPlayer)
	{
		// 이전에 FireAtPlayer() 같은 함수에서 계산해둔 플레이어 방향을 사용합니다.
		MoveDirection = ShootDirection;
	}
	else
	{
		// 기존처럼 액터가 바라보고 있는 정면 방향을 사용합니다.
		MoveDirection = GetActorForwardVector();
	}

	// 2. 결정된 방향을 바탕으로 다음 위치를 계산합니다. (기존 코드 완벽 재활용!)
	FVector NewLocation = GetActorLocation() + MoveDirection * Speed * DeltaTime;

	FHitResult Hit;

	SetActorLocation(NewLocation, true, &Hit); // bSweep = true to check collision

	// (주의: Collision 포인터가 유효한지 먼저 체크해 주는 것이 안전합니다)
	if (Collision)
	{
		Collision->UpdateOverlaps();
	}

	// 3. 충돌 처리 (기존 코드)
	if (Hit.IsValidBlockingHit())
	{
		DestroyProjectile();
	}

	// 4. 디버그 박스 그리기 (기존 코드 수정)
	if (Collision)
	{
		DrawDebugBox(GetWorld(), Collision->GetComponentLocation(), Collision->GetScaledBoxExtent(), Collision->GetComponentQuat(), FColor::Red, false, -1.0f, 0, 2.0f);
	}

}

void AEnemyProjectile::KnockbackPlayer(AActor* OtherActor)
{
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
}

void AEnemyProjectile::FireAtPlayer()
{
	// 1. 플레이어 캐릭터 찾기
	ACharacter* PlayerCharacter = UGameplayStatics::GetPlayerCharacter(this, 0);

	if (PlayerCharacter)
	{
		// 2. 시작점(투사체 위치)과 목표점(플레이어 위치) 구하기
		FVector StartLocation = GetActorLocation();

		// 플레이어의 가슴팍(중앙) 정도를 노리기 위해 Z축을 살짝 올려주면 더 좋아!
		FVector TargetLocation = PlayerCharacter->GetActorLocation() + FVector(0.0f, 0.0f, 30.0f);

		// 3. 방향 벡터 계산 및 정규화 (핵심 로직)
		// (목표점 - 시작점)을 하면 방향이 나오는데, 거리에 상관없이 속도를 일정하게 
		// 유지하기 위해 GetSafeNormal()로 길이를 1인 단위 벡터로 만들어 줘.
		ShootDirection = (TargetLocation - StartLocation).GetSafeNormal();

		// 4. (선택) 투사체가 날아가는 방향을 바라보도록 회전시키기
		// 화살이나 마취총 탄환처럼 앞뒤가 있는 모델링이라면 꼭 넣어줘.
		SetActorRotation(ShootDirection.Rotation());
	}

}


