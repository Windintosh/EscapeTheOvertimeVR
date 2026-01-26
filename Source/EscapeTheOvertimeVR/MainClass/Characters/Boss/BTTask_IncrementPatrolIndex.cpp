#include "BTTask_IncrementPatrolIndex.h"
#include "MainClass/Characters/Boss/BossAIController.h" // 보스 컨트롤러 헤더 경로 확인 필요
#include "BehaviorTree/BlackboardComponent.h"

UBTTask_IncrementPatrolIndex::UBTTask_IncrementPatrolIndex()
{
	NodeName = TEXT("Next Patrol Point");
}

EBTNodeResult::Type UBTTask_IncrementPatrolIndex::ExecuteTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory)
{
	ABossAIController* BossAI = Cast<ABossAIController>(OwnerComp.GetAIOwner());
	UBlackboardComponent* Blackboard = OwnerComp.GetBlackboardComponent();

	if (BossAI && Blackboard)
	{
		// 패트롤 포인트가 없으면 실패
		if (BossAI->PatrolPoints.Num() == 0)
		{
			return EBTNodeResult::Failed;
		}

		// 1. 현재 인덱스 가져오기
		int32 CurrentIndex = Blackboard->GetValueAsInt(ABossAIController::PatrolIndexKey);

		// 2. 다음 인덱스 계산 (나머지 연산으로 순환)
		// (0 -> 1 -> 2 -> 0 -> 1 ...)
		int32 NextIndex = (CurrentIndex + 1) % BossAI->PatrolPoints.Num();

		// 3. 블랙보드 업데이트 (인덱스 & 실제 벡터 좌표)
		Blackboard->SetValueAsInt(ABossAIController::PatrolIndexKey, NextIndex);
		Blackboard->SetValueAsVector(ABossAIController::PatrolLocationKey, BossAI->PatrolPoints[NextIndex]);

		return EBTNodeResult::Succeeded;
	}

	return EBTNodeResult::Failed;
}
