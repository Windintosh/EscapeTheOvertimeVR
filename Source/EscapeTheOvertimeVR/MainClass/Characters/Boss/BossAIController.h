#pragma once

#include "CoreMinimal.h"
#include "AIController.h"
#include "BehaviorTree/BehaviorTree.h"
#include "BehaviorTree/BlackboardComponent.h"
#include "Perception/AIPerceptionComponent.h"
#include "Perception/AISenseConfig_Sight.h"
#include "Perception/AISenseConfig_Hearing.h" 
#include "BossAIController.generated.h"

UCLASS()
class ESCAPETHEOVERTIMEVR_API ABossAIController : public AAIController
{
	GENERATED_BODY()

public:
	ABossAIController();

	// 패트롤 지점 목록
	TArray<FVector> PatrolPoints;

	UPROPERTY(EditAnywhere, Category = "AI")
	UBehaviorTree* BehaviorTree;

	// AIPerception Component (시각 + 청각 통합)
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "AI|Perception")
	UAIPerceptionComponent* BossAIPerceptionComponent;

	// Sight Config
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "AI|Perception")
	UAISenseConfig_Sight* SightConfig;

	// Hearing Config (PawnSensing 대체)
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "AI|Perception")
	UAISenseConfig_Hearing* HearingConfig;

protected:
	virtual void BeginPlay() override;
	virtual void OnPossess(APawn* InPawn) override;

	// 월드의 모든 TargetPoint를 찾아 PatrolPoints에 저장
	void InitializePatrolPoints();

	// 감각(시각/청각) 업데이트 시 호출되는 함수
	UFUNCTION()
	void OnTargetPerceptionUpdated(AActor* Actor, FAIStimulus Stimulus);

public:
	// Blackboard Keys (기존 키 활용)
	static const FName PatrolLocationKey;
	static const FName PatrolIndexKey;
	static const FName TargetPlayerKey;
	static const FName CanSeePlayerKey;
	static const FName LastSeenLocationKey;
	static const FName LastHeardLocationKey;
	static const FName CanHearPlayerKey;
	static const FName StunKey;

private:
	UBlackboardComponent* BB = nullptr;
};
