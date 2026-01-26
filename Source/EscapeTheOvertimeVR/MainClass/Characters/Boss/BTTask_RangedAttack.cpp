#include "MainClass/Characters/Boss/BTTask_RangedAttack.h"
#include "MainClass/Characters/Boss/MonkAIController.h"
#include "MainClass/Characters/Boss/MonkBoss.h"
#include "GameFramework/CharacterMovementComponent.h"

UBTTask_RangedAttack::UBTTask_RangedAttack()
{
	NodeName = TEXT("RangedAttack");
}

EBTNodeResult::Type UBTTask_RangedAttack::ExecuteTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory)
{
	// 1. AI 컨트롤러 가져오기
	ABossAIController* AIController = Cast<ABossAIController>(OwnerComp.GetAIOwner());
	if (!AIController) return EBTNodeResult::Failed;

	// 2. 조종 중인 보스 캐릭터 가져오기
	AMonkBoss* Boss = Cast<AMonkBoss>(AIController->GetPawn());
	if (!Boss) return EBTNodeResult::Failed;

	Boss->bUseControllerRotationYaw = true;

	if (Boss->GetCharacterMovement())
	{
		Boss->GetCharacterMovement()->bOrientRotationToMovement = false;
	}
	// 3. 공격 함수 호출
	Boss->Attack();

	// 4. 성공 반환 (Succeeded)
	return EBTNodeResult::Succeeded;
}

void UBTTask_RangedAttack::OnTaskFinished(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory, EBTNodeResult::Type TaskResult)
{
	Super::OnTaskFinished(OwnerComp, NodeMemory, TaskResult);

	AAIController* AIController = OwnerComp.GetAIOwner();
	if (AIController)
	{
		AMonkBoss* Boss = Cast<AMonkBoss>(AIController->GetPawn());
		if (Boss)
		{
			// [복구] 다시 이동 방향을 바라보게 설정
			Boss->bUseControllerRotationYaw = false;

			if (Boss->GetCharacterMovement())
			{
				Boss->GetCharacterMovement()->bOrientRotationToMovement = true;
			}
		}
	}
}
