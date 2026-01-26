#include "MainClass/Characters/Boss/BTTask_SetFocus.h"
#include "AIController.h"
#include "BehaviorTree/BlackboardComponent.h"

const FName UBTTask_SetFocus::TargetPlayerKey = TEXT("TargetPlayer");

UBTTask_SetFocus::UBTTask_SetFocus()
{
	NodeName = TEXT("Set Focus To Target");
}

EBTNodeResult::Type UBTTask_SetFocus::ExecuteTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory)
{
	// 1. AI 컨트롤러 가져오기
	AAIController* AIController = OwnerComp.GetAIOwner();
	if (!AIController)
	{
		return EBTNodeResult::Failed;
	}

	// 2. 블랙보드 컴포넌트 가져오기
	UBlackboardComponent* BlackboardComp = OwnerComp.GetBlackboardComponent();
	if (!BlackboardComp)
	{
		return EBTNodeResult::Failed;
	}

	// 3. 블랙보드 키(TargetActor)에서 오브젝트(Actor) 가져오기
	// GetSelectedBlackboardKey()는 에디터에서 선택한 키의 ID를 반환합니다.
	UObject* TargetObject = BlackboardComp->GetValueAsObject(TargetPlayerKey);
	AActor* TargetActor = Cast<AActor>(TargetObject);

	if (TargetActor)
	{
		// 4. Set Focus 실행 (이제부터 AI는 이 액터를 계속 바라봅니다)
		AIController->SetFocus(TargetActor);

		// 명령을 내렸으므로 성공 반환
		return EBTNodeResult::Succeeded;
	}
	else
	{
		// 타겟이 없다면 Focus를 해제하는 것이 안전할 수 있습니다.
		// 상황에 따라 다르지만, 타겟을 잃어버린 경우라면 ClearFocus를 호출하기도 합니다.
		AIController->ClearFocus(EAIFocusPriority::Gameplay);
		return EBTNodeResult::Failed;
	}
}
