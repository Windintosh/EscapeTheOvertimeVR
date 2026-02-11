#pragma once

#include "CoreMinimal.h"
#include "BehaviorTree/BTTaskNode.h"
#include "BTTask_StunAnim.generated.h"

/**
 * 
 */
UCLASS()
class ESCAPETHEOVERTIMEVR_API UBTTask_StunAnim : public UBTTaskNode
{
	GENERATED_BODY()
	
public:
	UBTTask_StunAnim();

protected:
	virtual EBTNodeResult::Type ExecuteTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory) override;
};
