#include "MainClass/Characters/Creatures/DoggyX.h"
#include "Engine/OverlapResult.h" // 충돌 결과 처리용
#include "DrawDebugHelpers.h"     // 디버그 원 그리기용
#include "MainClass/Characters/VRCharacterPawn/VRCharacterPawn.h"

ADoggyX::ADoggyX()
{

}

void ADoggyX::BeginPlay()
{
	Super::BeginPlay();
}

void ADoggyX::Attack()
{
	FVector StartLocation = GetActorLocation();
	FCollisionShape CollisionShape = FCollisionShape::MakeSphere(100.f);

	FCollisionQueryParams Params;
	Params.AddIgnoredActor(this); // 나 자신은 무시

	TArray<FOverlapResult> OverlapResults;
	bool bHit = GetWorld()->OverlapMultiByChannel(
		OverlapResults,
		StartLocation,
		FQuat::Identity,
		ECC_Pawn, // 플레이어 캐릭터를 감지하기 위한 채널
		CollisionShape,
		Params
	);

	DrawDebugSphere(GetWorld(), StartLocation, 100.f, 12, bHit ? FColor::Red : FColor::Blue, false, 1.0f);

	if(bHit)
	{
		for (const FOverlapResult& Result : OverlapResults)
		{
			AActor* HitActor = Result.GetActor();
			if(HitActor && HitActor->IsA(AVRCharacterPawn::StaticClass())) //or change to AActor or sth
			{
				UE_LOG(LogTemp, Warning, TEXT("DoggyX attacked: %s"), *HitActor->GetName());
				// 여기에 데미지 처리 로직 추가 가능
				HitActor->TakeDamage(20.f, FDamageEvent(), GetController(), this);
			}
		}
	}
	//effects, sound 등 추가 가능

	Destroy(); // 공격 후 자신을 제거
}
