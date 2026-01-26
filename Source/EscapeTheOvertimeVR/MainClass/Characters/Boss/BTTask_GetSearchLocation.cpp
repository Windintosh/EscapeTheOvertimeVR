#include "MainClass/Characters/Boss/BTTask_GetSearchLocation.h"
#include "BehaviorTree/BlackboardComponent.h"
#include "SearchPointComponent.h"

const FName UBTTask_GetSearchLocation::TargetPlayerKey = TEXT("TargetPlayer");
const FName UBTTask_GetSearchLocation::PatrolLocationKey = TEXT("PatrolLocation");

UBTTask_GetSearchLocation::UBTTask_GetSearchLocation()
{
    NodeName = TEXT("Get Search Location");
}

EBTNodeResult::Type UBTTask_GetSearchLocation::ExecuteTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory)
{
    UObject* RawObject = OwnerComp.GetBlackboardComponent()->GetValueAsObject("TargetPlayer"); // EQS 결과
    AActor* TargetActor = Cast<AActor>(RawObject);

    if (TargetActor)
    {
        USearchPointComponent* SearchComp = TargetActor->FindComponentByClass<USearchPointComponent>();
        if (SearchComp)
        {
            // 컴포넌트의 정확한 위치를 벡터 값으로 블랙보드에 저장
            OwnerComp.GetBlackboardComponent()->SetValueAsVector(PatrolLocationKey, SearchComp->GetComponentLocation()); //moveto node
            return EBTNodeResult::Succeeded;
        }
    }
    return EBTNodeResult::Failed;
}
