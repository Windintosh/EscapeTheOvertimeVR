#include "MainClass/Objects/Props/RobotVacuum/RobotVacuum.h"
#include "TQAmmo.h"
#include "Kismet/KismetMathLibrary.h" // 랜덤 위치 계산용

ARobotVacuum::ARobotVacuum()
{
	PrimaryActorTick.bCanEverTick = true;
	bCanMove = true; // 기본적으로 움직임 허용
    Collision->SetCollisionProfileName(TEXT("BlockAllDynamic"));
}

AMainDoorKey* ARobotVacuum::SpawnKey()
{
    if (bIsKeySpawned) return nullptr;
	PrimaryActorTick.bCanEverTick = false;
	SpawnedKey = GetWorld()->SpawnActor<AMainDoorKey>(MainDoorKeyClass, GetActorLocation(), GetActorRotation());
	bIsKeySpawned = true;
	return SpawnedKey;
}

void ARobotVacuum::BeginPlay()
{
	Super::BeginPlay();

	PickNextTarget();
}

void ARobotVacuum::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	FRotator CurrentRotation = GetActorRotation();
    if ((CurrentRotation.Roll > 90) || (CurrentRotation.Pitch > 90) && !bIsKeySpawned)
    {
        StopRVMovement();
        if(!bIsKeySpawned) SpawnKey();
        bIsKeySpawned = true;
		PrimaryActorTick.bCanEverTick = false;
    }

    // 1. 움직일 수 없는 상태면(잡힘 등) 아무것도 안 함
    if (!bCanMove) return;

    // 2. 현재 위치 가져오기
    FVector CurrentLocation = GetActorLocation();

    // 3. 목표 지점까지 일정한 속도로 이동할 위치 계산
    // VInterpConstantTo는 부드럽게 가속하지 않고 일정한 속도(등속 운동)로 이동합니다.
    FVector NewLocation = FMath::VInterpConstantTo(CurrentLocation, TargetLocation, DeltaTime, MoveSpeed);

    // [추가] 회전 로직: 이동하려는 방향 벡터 구하기
    FVector Direction = (TargetLocation - CurrentLocation).GetSafeNormal();

    if (!Direction.IsZero())
    {
        // 목표 방향의 회전값 계산
        FRotator TargetRotation = Direction.Rotation();

        // 현재 회전값에서 목표 회전값으로 부드럽게 회전 (RInterpTo)
        // RotationSpeed는 헤더에 변수로 추가 필요 (예: 5.0f)
        FRotator NewRotation = FMath::RInterpTo(GetActorRotation(), TargetRotation, DeltaTime, 5.0f);

        SetActorRotation(NewRotation);
    }

    // 4. 위치 설정 (bSweep=true로 설정하여 벽에 부딪히면 멈추게 함)
    FHitResult Hit;
    SetActorLocation(NewLocation, true, &Hit);

    // 5. 목표 지점 도착 확인 (거리 계산)
    float Distance = FVector::Dist(CurrentLocation, TargetLocation);

    // 목표에 거의 도달했거나, 벽에 부딪혔다면 다음 목표 설정
    if (Distance < AcceptanceRadius || Hit.bBlockingHit)
    {
        PickNextTarget();
    }
}

void ARobotVacuum::Grab_Implementation(USceneComponent* HandController)
{
	Super::Grab_Implementation(HandController);

    StopRVMovement();
	//ActivateProp(HandController->GetOwner());
}

void ARobotVacuum::Release_Implementation(FVector ThrowVelocity)
{
	Super::Release_Implementation(ThrowVelocity);
}

void ARobotVacuum::OnPropOverlap(UPrimitiveComponent* OverlappedComp, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{
    AActor* Ammo = Cast<AActor>(OtherActor);
	if (Ammo->GetClass()->GetName().Contains(TEXT("Projectile")))
	{
		ActivateProp(OtherActor);
        StopRVMovement();
        return;
	}
    //StartRVMovement();
    //StopRVMovement();
}

void ARobotVacuum::ActivateProp(AActor* Activator)
{
	StopRVMovement();
}

void ARobotVacuum::StartRVMovement()
{
    bCanMove = true;
    PickNextTarget(); // 멈췄던 곳이 아니라 새로운 곳으로 가고 싶다면 호출
}

void ARobotVacuum::StopRVMovement()
{
	//play sound or effect here
    bCanMove = false;
	//PrimaryActorTick.bCanEverTick = false;
    // 필요하다면 여기서 잡히는 애니메이션 재생이나 사운드 출력 등을 추가
}

void ARobotVacuum::PickNextTarget()
{
    // 현재 위치를 기준으로 랜덤한 반경 내의 점을 목표로 설정
    // *순찰(Patrol)을 원하시면 이 부분을 미리 지정된 배열의 좌표로 바꾸면 됩니다.
    TargetLocation = GetActorLocation() + UKismetMathLibrary::RandomUnitVector() * FMath::FRandRange(200.0f, RandomMoveRadius);

    // 높이(Z축)는 현재 높이 유지 (바닥에서 뜨지 않게)
    TargetLocation.Z = GetActorLocation().Z;
}
