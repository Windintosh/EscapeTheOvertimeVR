#include "MainClass/Characters/Creatures/Monkey.h"
#include "Kismet/GameplayStatics.h"
#include "GameFramework/ProjectileMovementComponent.h"

void AMonkey::ThrowBanana()
{
    // 1. 시작 위치 (원숭이 손의 소켓)
    FVector StartLocation = GetMesh()->GetSocketLocation(TEXT("BananaSocket"));

    // 2. 목표 위치 (플레이어의 위치)
    // 플레이어의 중앙(가슴춤)을 맞추고 싶다면 Z축으로 살짝 올려주는 센스가 필요해요!
    ACharacter* Player = UGameplayStatics::GetPlayerCharacter(this, 0);
    if (!Player) return;

    FVector EndLocation = Player->GetActorLocation();
    // FVector EndLocation = Player->GetActorLocation() + FVector(0, 0, 50.0f); // 가슴 조준

    // 3. 발사 속도를 담을 변수
    FVector LaunchVelocity;

    // 4. 마법의 함수: "Start에서 End로 던지려면 어떤 속도로 던져야 해?"
    // TossSpeed(던지는 힘)를 정해주면, 엔진이 알맞은 각도(Velocity)를 계산해서 LaunchVelocity에 넣어줍니다.
    //float TossSpeed = 1200.0f; // 이 값을 조절해서 포물선의 높낮이를 결정하세요!

    bool bSuccess = UGameplayStatics::SuggestProjectileVelocity(
        this,
        LaunchVelocity,      // 계산된 결과가 여기에 담깁니다.
        StartLocation,       // 시작점
        EndLocation,         // 도착점
        TossSpeed,           // 던지는 힘 (너무 낮으면 목표까지 안 닿아서 실패할 수 있어요)
        false,               // 높은 포물선을 원하면 true, 빠르고 낮은 직선 위주면 false
        0.0f,
        0.0f,
        ESuggestProjVelocityTraceOption::DoNotTrace // 중간에 장애물이 있는지 검사할지 여부
    );

    // 5. 계산에 성공했다면 바나나를 스폰하고 던집니다!
    if (bSuccess)
    {
        // 바나나가 날아갈 방향(회전값)을 속도 벡터에서 추출
        FRotator SpawnRotation = LaunchVelocity.Rotation();

        // 바나나 소환
        AActor* SpawnedBanana = GetWorld()->SpawnActor<AActor>(BananaClass, StartLocation, SpawnRotation);

        if (SpawnedBanana)
        {
            // 소환된 바나나의 ProjectileMovement 컴포넌트를 찾아 속도를 적용!
            UProjectileMovementComponent* ProjComp = SpawnedBanana->FindComponentByClass<UProjectileMovementComponent>();
            if (ProjComp)
            {
                ProjComp->Velocity = LaunchVelocity; // 계산된 완벽한 포물선 궤적 적용
            }
        }
    }
    else
    {
        // 목표가 너무 멀거나 던질 수 없는 각도일 때의 예외 처리
        UE_LOG(LogTemp, Warning, TEXT("원숭이: 닿지 않는 거리야!"));
    }
}
