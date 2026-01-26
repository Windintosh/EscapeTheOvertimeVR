#pragma once

#include "CoreMinimal.h"
#include "MainClass/Characters/Boss/BossAIController.h"
#include "MonkAIController.generated.h"

/**
 * 
 */
UCLASS()
class ESCAPETHEOVERTIMEVR_API AMonkAIController : public ABossAIController
{
	GENERATED_BODY()
	
public:
	AMonkAIController();

	virtual void BeginPlay() override;
	virtual void OnPossess(APawn* InPawn) override;

private:
	UBlackboardComponent* BB = nullptr;
};
