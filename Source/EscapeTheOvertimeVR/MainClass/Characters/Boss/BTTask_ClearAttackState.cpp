#include "MainClass/Characters/Boss/BTTask_ClearAttackState.h"
#include "BossAIController.h"
#include "Boss.h"

UBTTask_ClearAttackState::UBTTask_ClearAttackState()
{
	NodeName = TEXT("Clear Attack State");
}

EBTNodeResult::Type UBTTask_ClearAttackState::ExecuteTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory)
{
	// 1. AI 컨트롤러 가져오기
	ABossAIController* AIController = Cast<ABossAIController>(OwnerComp.GetAIOwner());
	if (!AIController)
	{
		UE_LOG(LogTemp, Error, TEXT("Attack: No AIController found"));
		return EBTNodeResult::Failed;
	}

	// 2. 조종 중인 보스 캐릭터 가져오기
	ABoss* Boss = Cast<ABoss>(AIController->GetPawn());
	if (!Boss)
	{
		UE_LOG(LogTemp, Error, TEXT("Attack: No BossCharacter found"));
		return EBTNodeResult::Failed;
	}

	Boss->bIsAttacking = false;

	return EBTNodeResult::Succeeded;
}

