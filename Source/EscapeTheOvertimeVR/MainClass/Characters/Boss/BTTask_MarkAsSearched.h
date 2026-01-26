#pragma once

#include "CoreMinimal.h"
#include "BehaviorTree/Tasks/BTTask_BlackboardBase.h"
#include "BTTask_MarkAsSearched.generated.h"

/**
 * 
 */
UCLASS()
class ESCAPETHEOVERTIMEVR_API UBTTask_MarkAsSearched : public UBTTask_BlackboardBase
{
	GENERATED_BODY()
	
public:
	UBTTask_MarkAsSearched();
	virtual EBTNodeResult::Type ExecuteTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory) override;

	static const FName TargetPlayerKey;
};
