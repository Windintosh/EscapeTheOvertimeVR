#include "CleanerBoss.h"
#include "BossAIController.h" 
#include "BehaviorTree/BlackboardComponent.h"

void ACleanerBoss::BeginPlay()
{
	Super::BeginPlay();

	// 1. 게임 시작 시 서 있는 위치와 방향을 기억함
	HomeLocation = GetActorLocation();
	HomeRotation = GetActorRotation();

	// 2. 블랙보드에 초기 위치 전송 (AI Controller가 빙의된 후라고 가정)
	if (ABossAIController* AIController = Cast<ABossAIController>(GetController()))
	{
		if (AIController->GetBlackboardComponent())
		{
			AIController->GetBlackboardComponent()->SetValueAsVector(TEXT("HomeLocation"), HomeLocation);
			AIController->GetBlackboardComponent()->SetValueAsRotator(TEXT("HomeRotation"), HomeRotation);
		}
	}
}
