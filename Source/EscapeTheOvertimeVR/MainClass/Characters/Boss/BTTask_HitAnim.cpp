#include "MainClass/Characters/Boss/BTTask_HitAnim.h"
#include "MainClass/Characters/Boss/BossAIController.h"
#include "MainClass/Characters/Boss/Boss.h"

UBTTask_HitAnim::UBTTask_HitAnim()
{
	NodeName = TEXT("HitAnim");
}

EBTNodeResult::Type UBTTask_HitAnim::ExecuteTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory)
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

	// 3. 피격 애니메이션 재생 함수 호출
	if(Boss->HitMontage)
	{
		UAnimInstance* AnimInstance = Boss->GetMesh()->GetAnimInstance();
		if (AnimInstance)
		{
			AnimInstance->Montage_Play(Boss->HitMontage);
			UE_LOG(LogTemp, Warning, TEXT("Boss: Playing Hit Animation!"));
			//return EBTNodeResult::Succeeded;
		}
		else
		{
			UE_LOG(LogTemp, Error, TEXT("HitAnim: No AnimInstance found"));
			return EBTNodeResult::Failed;
		}
	}
	else
	{
		UE_LOG(LogTemp, Error, TEXT("HitAnim: No HitMontage assigned"));
		return EBTNodeResult::Failed;
	}
	return EBTNodeResult::Succeeded;
}
