#include "MainClass/Characters/Creatures/MonkeyAIController.h"

AMonkeyAIController::AMonkeyAIController()
{
	SightConfig->SightRadius = 1500.0f; // 감지 반경
	SightConfig->LoseSightRadius = 2000.0f; // same as original boss

	BossAIPerceptionComponent->ConfigureSense(*SightConfig);
}
