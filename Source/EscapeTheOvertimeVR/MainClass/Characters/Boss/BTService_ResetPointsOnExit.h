#pragma once

#include "CoreMinimal.h"
#include "BehaviorTree/BTService.h"
#include "BTService_ResetPointsOnExit.generated.h"

/**
 * 
 */
UCLASS()
class ESCAPETHEOVERTIMEVR_API UBTService_ResetPointsOnExit : public UBTService
{
	GENERATED_BODY()
	
public:
	UBTService_ResetPointsOnExit();

	// 서비스가 활성화된 노드(수색)에서 "나갈 때" 실행되는 함수
	virtual void OnCeaseRelevant(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory) override;
};
