#include "BTTask_Clean.h"
#include "MainClass/Characters/Boss/CleanerBoss.h" 
#include "AIController.h"
#include "GameFramework/Character.h"

UBTTask_Clean::UBTTask_Clean()
{
	NodeName = TEXT("Clean Elevator");
	bNotifyTick = true;
}

EBTNodeResult::Type UBTTask_Clean::ExecuteTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory)
{
	AAIController* AIController = OwnerComp.GetAIOwner();

	// 컨트롤러나 폰이 없으면 실패 처리
	if (!AIController) return EBTNodeResult::Failed;

	ACleanerBoss* CleanerBoss = Cast<ACleanerBoss>(AIController->GetPawn());
	if (!CleanerBoss) return EBTNodeResult::Failed;

	// 1. 청소 위치(Home)에서의 정확한 각도로 회전
	// (MoveTo가 도착해도 각도가 틀어져 있을 수 있으므로 강제 정렬)
	CleanerBoss->SetActorRotation(CleanerBoss->HomeRotation);

	// 2. 청소 애니메이션 몽타주 재생 시작
	if (CleanerBoss->CleaningMontage)
	{
		CleanerBoss->PlayAnimMontage(CleanerBoss->CleaningMontage);
	}

	// 3. '진행 중(InProgress)' 상태 반환
	// Succeeded를 반환하면 트리가 끝나버리므로, 계속 청소하게 하기 위해 InProgress를 유지
	// 나중에 플레이어를 발견하면 BT Decorator가 이 태스크를 Abort(중단) 
	return EBTNodeResult::InProgress;
}

void UBTTask_Clean::TickTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory, float DeltaSeconds)
{
	Super::TickTask(OwnerComp, NodeMemory, DeltaSeconds);

	AAIController* AIController = OwnerComp.GetAIOwner();
	if (!AIController)
	{
		FinishLatentTask(OwnerComp, EBTNodeResult::Failed);
		return;
	}

	ACleanerBoss* CleanerBoss = Cast<ACleanerBoss>(AIController->GetPawn());
	if (!CleanerBoss)
	{
		FinishLatentTask(OwnerComp, EBTNodeResult::Failed);
		return;
	}

	// 몽타주가 끝났는지 확인하고, 끝났으면 다시 재생 (무한 반복)
	// (만약 몽타주 파일 자체에 Loop 설정이 되어 있다면 이 로직은 안전장치 역할을 함)
	UAnimInstance* AnimInstance = CleanerBoss->GetMesh()->GetAnimInstance();
	if (AnimInstance && CleanerBoss->CleaningMontage)
	{
		if (!AnimInstance->Montage_IsPlaying(CleanerBoss->CleaningMontage))
		{
			CleanerBoss->PlayAnimMontage(CleanerBoss->CleaningMontage);
		}
	}
}
