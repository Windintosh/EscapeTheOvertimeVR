#include "MainClass/Objects/WorldObjects/EnvQueryGenerator_Searchables.h"
#include "SearchPointComponent.h"
#include "EnvironmentQuery/Items/EnvQueryItemType_Actor.h"
#include "GameFramework/Actor.h"

UEnvQueryGenerator_Searchables::UEnvQueryGenerator_Searchables()
{
    // [생성자에서 기본값 설정]
    SearchRadius.DefaultValue = 1500.0f;

    // 반환할 아이템 타입 설정
    ItemType = UEnvQueryItemType_Actor::StaticClass();
}

void UEnvQueryGenerator_Searchables::GenerateItems(FEnvQueryInstance& QueryInstance) const
{
    // 1. 쿼리 요청자(AI) 위치 파악
    UObject* QueryOwner = QueryInstance.Owner.Get();
    if (!QueryOwner) return;
    FVector CenterLocation = Cast<AActor>(QueryOwner)->GetActorLocation();
    float Radius = SearchRadius.GetValue();

    TArray<AActor*> MatchingActors;

    // 2. 월드의 모든 SearchPointComponent를 순회 (최적화를 위해선 별도의 관리자 클래스를 두는게 좋지만, 일단 기본 방식으로 구현)
    for (TObjectIterator<USearchPointComponent> It; It; ++It)
    {
        USearchPointComponent* Comp = *It;

        // 유효하고, 월드에 존재하며, 아직 수색 안 된 곳만
        if (Comp && Comp->GetWorld() == QueryOwner->GetWorld() && !Comp->bIsSearched)
        {
            // 거리 체크
            if (FVector::Dist(Comp->GetComponentLocation(), CenterLocation) <= Radius)
            {
                // 컴포넌트의 주인 액터(예: 책상)를 결과 목록에 추가
                MatchingActors.Add(Comp->GetOwner());
            }
        }
    }

    // 3. EQS 시스템에 아이템(액터들) 전달
    QueryInstance.AddItemData<UEnvQueryItemType_Actor>(MatchingActors);
}
