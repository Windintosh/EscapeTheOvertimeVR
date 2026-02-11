#pragma once

#include "CoreMinimal.h"
#include "BehaviorTree/BTTaskNode.h"
#include "BTTask_HitAnim.generated.h"

/**
 * 
 */
UCLASS()
class ESCAPETHEOVERTIMEVR_API UBTTask_HitAnim : public UBTTaskNode
{
	GENERATED_BODY()
	
public:
	UBTTask_HitAnim();

protected:
	virtual EBTNodeResult::Type ExecuteTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory) override;
};
