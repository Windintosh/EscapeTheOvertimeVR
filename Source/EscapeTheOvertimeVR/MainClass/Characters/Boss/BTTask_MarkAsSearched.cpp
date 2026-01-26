#include "MainClass/Characters/Boss/BTTask_MarkAsSearched.h"
#include "SearchPointComponent.h"
#include "BehaviorTree/BlackboardComponent.h"
#include "GameFramework/Actor.h"

const FName UBTTask_MarkAsSearched::TargetPlayerKey = TEXT("TargetPlayer");

UBTTask_MarkAsSearched::UBTTask_MarkAsSearched()
{
	NodeName = "Mark As Searched";
	// 블랙보드 키 필터 (Actor만 허용)
	//BlackboardKey.AddObjectFilter(this, GET_MEMBER_NAME_CHECKED(UBTTask_MarkAsSearched, BlackboardKey), AActor::StaticClass());
}

EBTNodeResult::Type UBTTask_MarkAsSearched::ExecuteTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory)
{
    // 1. 블랙보드에서 타겟 액터(책상 등)를 가져옴
    UObject* TargetObject = OwnerComp.GetBlackboardComponent()->GetValueAsObject(TargetPlayerKey);
    AActor* TargetActor = Cast<AActor>(TargetObject);

    if (TargetActor)
    {
        // 2. 컴포넌트를 찾아서 bIsSearched를 true로 변경
        USearchPointComponent* SearchComp = TargetActor->FindComponentByClass<USearchPointComponent>();
        if (SearchComp)
        {
            SearchComp->bIsSearched = true;

            // (디버그용) 로그 출력
            UE_LOG(LogTemp, Warning, TEXT("Marked %s as Searched!"), *TargetActor->GetName());

            return EBTNodeResult::Succeeded;
        }
    }

    return EBTNodeResult::Failed;
}
