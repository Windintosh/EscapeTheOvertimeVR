#include "MainClass/Characters/Dancer/DancerAIController.h"

const FName ADancerAIController::TargetPlayerKey = TEXT("TargetPlayer");
const FName ADancerAIController::CanSeePlayerKey = TEXT("CanSeePlayer");
const FName ADancerAIController::LastSeenLocationKey = TEXT("LastSeenLocation");

ADancerAIController::ADancerAIController()
{
	// 1. Perception 컴포넌트 생성
	DancerAIPerceptionComponent = CreateDefaultSubobject<UAIPerceptionComponent>(TEXT("PerceptionComponent"));

	// 2. 시각 설정 (Sight)
	SightConfig = CreateDefaultSubobject<UAISenseConfig_Sight>(TEXT("SightConfig"));
	SightConfig->SightRadius = 1500.0f; // 감지 반경
	SightConfig->LoseSightRadius = 2000.0f;
	SightConfig->PeripheralVisionAngleDegrees = 90.0f;
	SightConfig->SetMaxAge(5.0f); // 기억 유지 시간

	SightConfig->DetectionByAffiliation.bDetectEnemies = true;
	SightConfig->DetectionByAffiliation.bDetectFriendlies = true;
	SightConfig->DetectionByAffiliation.bDetectNeutrals = true;

	// 4. 설정 등록
	DancerAIPerceptionComponent->ConfigureSense(*SightConfig);
	DancerAIPerceptionComponent->SetDominantSense(SightConfig->GetSenseImplementation());
}

void ADancerAIController::BeginPlay()
{
	Super::BeginPlay();

	if (DancerAIPerceptionComponent)
	{
		DancerAIPerceptionComponent->OnTargetPerceptionUpdated.AddDynamic(this, &ADancerAIController::OnTargetPerceptionUpdated);
	}
}

void ADancerAIController::OnPossess(APawn* InPawn)
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

void ADancerAIController::OnTargetPerceptionUpdated(AActor* Actor, FAIStimulus Stimulus)
{
	if (!BB) return;
	if (Actor->ActorHasTag("Boss"))
	{
		UE_LOG(LogTemp, Display, TEXT("Dancer-OTPU: Found Boss!"));
	}
	else return;

	TSubclassOf<UAISense> SensedClass = UAIPerceptionSystem::GetSenseClassForStimulus(this, Stimulus);

	if (SensedClass == UAISense_Sight::StaticClass())
	{
		bool bCanSee = Stimulus.WasSuccessfullySensed();

		BB->SetValueAsBool(CanSeePlayerKey, bCanSee);

		if (bCanSee)
		{
			// Found boss and chase him;
			BB->SetValueAsObject(TargetPlayerKey, Actor);
			BB->SetValueAsVector(LastSeenLocationKey, Stimulus.StimulusLocation);

			UE_LOG(LogTemp, Warning, TEXT("Dancer-OTPU: Boss DETECTED! Chasing..."));
		}
		else
		{
			// 놓침 -> 추격 대상 해제 (하지만 LastSeenLocation에는 마지막 위치가 남아있음)
			BB->SetValueAsObject(TargetPlayerKey, nullptr);
			UE_LOG(LogTemp, Warning, TEXT("Dancer-OTPU: Boss LOST."));
		}
	}
}
