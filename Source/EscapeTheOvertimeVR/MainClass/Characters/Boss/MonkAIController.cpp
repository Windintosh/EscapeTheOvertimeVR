#include "MainClass/Characters/Boss/MonkAIController.h"

AMonkAIController::AMonkAIController()
{
	SightConfig->SightRadius = 4500.0f; // 감지 반경
	SightConfig->LoseSightRadius = 6000.0f; // x3 of original boss

	BossAIPerceptionComponent->ConfigureSense(*SightConfig);
}

void AMonkAIController::BeginPlay()
{
	Super::BeginPlay();


}

void AMonkAIController::OnPossess(APawn* InPawn)
{
	Super::OnPossess(InPawn);

	// 비헤이비어 트리 실행
	if (BehaviorTree)
	{
		UBlackboardComponent* BlackboardComp;
		if (UseBlackboard(BehaviorTree->BlackboardAsset, BlackboardComp))
		{
			BB = BlackboardComp; // 멤버 변수 BB 캐싱
			RunBehaviorTree(BehaviorTree);
		}
	}
}
