#pragma once

#include "CoreMinimal.h"
#include "BehaviorTree/BTTaskNode.h"
#include "BTTask_Clean.generated.h"

/**
 * 청소부 보스(CleanerBoss)가 엘리베이터를 닦는 행동을 수행하는 태스크
 */
UCLASS()
class ESCAPETHEOVERTIMEVR_API UBTTask_Clean : public UBTTaskNode
{
	GENERATED_BODY()

public:
	// 생성자
	UBTTask_Clean();

protected:
	// 태스크가 시작될 때 1회 호출 (회전 맞추기, 몽타주 재생 시작)
	virtual EBTNodeResult::Type ExecuteTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory) override;

	// 태스크가 실행 중일 때 매 프레임 호출 (몽타주가 끝났는지, 계속 재생할지 체크)
	// *생성자에서 bNotifyTick = true; 설정 필요
	virtual void TickTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory, float DeltaSeconds) override;
};
