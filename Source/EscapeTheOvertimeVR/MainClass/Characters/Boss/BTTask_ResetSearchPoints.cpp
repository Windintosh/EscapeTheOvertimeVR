#include "MainClass/Characters/Boss/BTTask_ResetSearchPoints.h"
#include "SearchPointComponent.h"
#include "UObject/UObjectIterator.h"

UBTTask_ResetSearchPoints::UBTTask_ResetSearchPoints()
{
	NodeName = "Reset Search Points";
}

EBTNodeResult::Type UBTTask_ResetSearchPoints::ExecuteTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory)
{
    // 월드에 존재하는 모든 SearchPointComponent를 순회합니다.
    // (이전 Generator에서 썼던 방식과 동일합니다)
    for (TObjectIterator<USearchPointComponent> It; It; ++It)
    {
        USearchPointComponent* SearchComp = *It;

        // 현재 월드에 있는 컴포넌트인지 확인 (에디터 프리뷰 등 제외)
        if (SearchComp && SearchComp->GetWorld() == OwnerComp.GetWorld())
        {
            // 수색 기록 초기화
            SearchComp->bIsSearched = false;
        }
    }

    // 작업 완료
    return EBTNodeResult::Succeeded;
}
