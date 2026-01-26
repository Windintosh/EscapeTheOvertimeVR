#include "BTTask_Attack.h"
#include "MainClass/Characters/Boss/BossAIController.h"
#include "MainClass/Characters/Boss/Boss.h"

UBTTask_Attack::UBTTask_Attack()
{
	NodeName = TEXT("Attack"); // 비헤이비어 트리에서 보일 이름
}

EBTNodeResult::Type UBTTask_Attack::ExecuteTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory)
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

	// 3. 공격 함수 호출
	Boss->Attack();
	UE_LOG(LogTemp, Error, TEXT("Attack: Attacking"));
	// 4. 성공 반환 (Succeeded)
	return EBTNodeResult::Succeeded;
}
