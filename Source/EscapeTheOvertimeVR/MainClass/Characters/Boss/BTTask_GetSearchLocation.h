#pragma once

#include "CoreMinimal.h"
#include "BehaviorTree/Tasks/BTTask_BlackboardBase.h"
#include "BTTask_GetSearchLocation.generated.h"

/**
 * 
 */
UCLASS()
class ESCAPETHEOVERTIMEVR_API UBTTask_GetSearchLocation : public UBTTask_BlackboardBase
{
	GENERATED_BODY()
	
	
public:
	UBTTask_GetSearchLocation();

	virtual EBTNodeResult::Type ExecuteTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory) override;

	static const FName TargetPlayerKey;
	static const FName PatrolLocationKey;
};
