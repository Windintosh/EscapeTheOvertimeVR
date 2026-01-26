#include "MainClass/Characters/Boss/BTService_ResetPointsOnExit.h"
#include "SearchPointComponent.h"
#include "UObject/UObjectIterator.h"

UBTService_ResetPointsOnExit::UBTService_ResetPointsOnExit()
{
    NodeName = "Reset Search On Exit";

    // 이 서비스는 틱(Tick)마다 실행될 필요는 없고, 나갈 때만 필요하므로
    bNotifyCeaseRelevant = true; // 나갈 때 알림 받기
    bNotifyTick = false;         // 매 프레임 실행 X (성능 최적화)
}

void UBTService_ResetPointsOnExit::OnCeaseRelevant(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory)
{
    Super::OnCeaseRelevant(OwnerComp, NodeMemory);

    // Reset Logic
    for (TObjectIterator<USearchPointComponent> It; It; ++It)
    {
        USearchPointComponent* SearchComp = *It;
        if (SearchComp && SearchComp->GetWorld() == OwnerComp.GetWorld())
        {
            SearchComp->bIsSearched = false;
        }
    }

    // (디버그용) 로그
    // UE_LOG(LogTemp, Log, TEXT("수색 모드 종료! 모든 기록을 초기화했습니다."));
}


