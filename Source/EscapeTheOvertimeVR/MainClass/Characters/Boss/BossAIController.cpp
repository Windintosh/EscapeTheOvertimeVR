#include "MainClass/Characters/Boss/BossAIController.h"
#include "Kismet/GameplayStatics.h"
#include "Engine/TargetPoint.h"
#include "GameFramework/Character.h"
#include "Perception/AISense_Sight.h"
#include "Perception/AISense_Hearing.h" // 청각 센스 헤더
#include "Boss/Boss.h"

// 블랙보드 키 정의
const FName ABossAIController::PatrolLocationKey = TEXT("PatrolLocation");
const FName ABossAIController::PatrolIndexKey = TEXT("PatrolIndex");
const FName ABossAIController::TargetPlayerKey = TEXT("TargetPlayer");
const FName ABossAIController::LastSeenLocationKey = TEXT("LastSeenLocation");
const FName ABossAIController::CanSeePlayerKey = TEXT("CanSeePlayer");
const FName ABossAIController::LastHeardLocationKey = TEXT("LastHeardLocation");
const FName ABossAIController::CanHearPlayerKey = TEXT("CanHearPlayer");
const FName ABossAIController::StunKey = TEXT("IsStunned");

ABossAIController::ABossAIController()
{
	PrimaryActorTick.bCanEverTick = true; // 디버깅용 외엔 false

	// 1. Perception 컴포넌트 생성
	BossAIPerceptionComponent = CreateDefaultSubobject<UAIPerceptionComponent>(TEXT("PerceptionComponent"));

	// 2. 시각 설정 (Sight)
	SightConfig = CreateDefaultSubobject<UAISenseConfig_Sight>(TEXT("SightConfig"));
	SightConfig->SightRadius = 500.0f; // 감지 반경
	SightConfig->LoseSightRadius = 700.0f;
	SightConfig->PeripheralVisionAngleDegrees = 60.0f;
	SightConfig->SetMaxAge(3.0f); // 기억 유지 시간

	SightConfig->DetectionByAffiliation.bDetectEnemies = true;
	SightConfig->DetectionByAffiliation.bDetectFriendlies = true;
	SightConfig->DetectionByAffiliation.bDetectNeutrals = true;

	// 3. 청각 설정 (Hearing) 
	HearingConfig = CreateDefaultSubobject<UAISenseConfig_Hearing>(TEXT("HearingConfig"));
	HearingConfig->HearingRange = 500.0f; // 소리 감지 범위
	HearingConfig->DetectionByAffiliation.bDetectEnemies = true;
	HearingConfig->DetectionByAffiliation.bDetectFriendlies = true;
	HearingConfig->DetectionByAffiliation.bDetectNeutrals = true;

	// 4. 설정 등록
	BossAIPerceptionComponent->ConfigureSense(*SightConfig);
	BossAIPerceptionComponent->ConfigureSense(*HearingConfig);
	BossAIPerceptionComponent->SetDominantSense(SightConfig->GetSenseImplementation());
}

void ABossAIController::BeginPlay()
{
	Super::BeginPlay();

	// 감각 업데이트 델리게이트 바인딩 (시각+청각 통합 처리)
	if (BossAIPerceptionComponent)
	{
		BossAIPerceptionComponent->OnTargetPerceptionUpdated.AddDynamic(this, &ABossAIController::OnTargetPerceptionUpdated);
	}
}

void ABossAIController::OnPossess(APawn* InPawn)
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

	// 0.5초 뒤 패트롤 지점 초기화 (월드 로딩 대기)
	FTimerHandle TimerHandle;
	GetWorldTimerManager().SetTimer(TimerHandle, this, &ABossAIController::InitializePatrolPoints, 0.5f, false);
}

void ABossAIController::InitializePatrolPoints()
{
	PatrolPoints.Empty();

	// 월드에 배치된 TargetPoint 액터들을 모두 찾음
	TArray<AActor*> FoundTargetPoints;
	UGameplayStatics::GetAllActorsOfClass(GetWorld(), ATargetPoint::StaticClass(), FoundTargetPoints);

	for (AActor* TargetPoint : FoundTargetPoints)
	{
		PatrolPoints.Add(TargetPoint->GetActorLocation());
	}

	UE_LOG(LogTemp, Warning, TEXT("AI: PatrolPoints Found: %d"), PatrolPoints.Num());

	// 첫 번째 패트롤 지점 설정
	if (PatrolPoints.Num() > 0 && BB)
	{
		BB->SetValueAsVector(PatrolLocationKey, PatrolPoints[0]);
		BB->SetValueAsInt(PatrolIndexKey, 0);
	}
}

// 통합 감각 처리 함수 (시각 + 청각)
void ABossAIController::OnTargetPerceptionUpdated(AActor* Actor, FAIStimulus Stimulus)
{
    if (!BB) return;

    bool bIsStunned = BB->GetValueAsBool(StunKey);

    if (BB->GetValueAsBool(StunKey)) return; //if AICharacter is stunned, do not update perception

    if (bIsStunned)
    {
        UE_LOG(LogTemp, Warning, TEXT("AI is Stunned! Ignoring Perception Update."));
    }

    if (bIsStunned)
    {
        // [핵심] 기절 상태인데 감각 정보가 들어왔다면? -> 강제로 잊게 만듦
        // 이걸 안 하면 다음 프레임에 또 들어올 수 있음
        BossAIPerceptionComponent->ForgetActor(Actor);
        return;
    }

    // 플레이어인지 확인
    ACharacter* PlayerCharacter = UGameplayStatics::GetPlayerCharacter(GetWorld(), 0);
    if (!PlayerCharacter || Actor != PlayerCharacter) return;

    // 감각 종류 식별
    TSubclassOf<UAISense> SensedClass = UAIPerceptionSystem::GetSenseClassForStimulus(this, Stimulus);

    // 1. [시각] 눈으로 봤을 때
    if (SensedClass == UAISense_Sight::StaticClass())
    {
        bool bCanSee = Stimulus.WasSuccessfullySensed();

        // 시야 여부(CanSeePlayer) 업데이트
        // (이건 매번 업데이트해도 보통 데코레이터가 Abort를 유발하지 않으므로 괜찮음, 
        // 하지만 최적화를 원하면 여기도 체크 가능)
        BB->SetValueAsBool(CanSeePlayerKey, bCanSee);

        if (bCanSee)
        {
            // [중요 수정] 버벅임 방지 로직
            // 현재 블랙보드에 저장된 타겟이 지금 본 녀석과 같은지 확인
            UObject* CurrentTarget = BB->GetValueAsObject(TargetPlayerKey);

            // "저장된 타겟이 없거나" OR "다른 대상을 봤을 때만" 값을 갱신
            if (CurrentTarget != Actor)
            {
                BB->SetValueAsObject(TargetPlayerKey, Actor);
                UE_LOG(LogTemp, Warning, TEXT("AI: Player SIGHTED! Chasing..."));
            }

            // 위치 정보는 계속 업데이트해줘야 정확히 쫓아감
            BB->SetValueAsVector(LastSeenLocationKey, Stimulus.StimulusLocation);

            // 청각 관련 키 초기화 (눈으로 봤으니 소리 추적보다 우선)
            BB->SetValueAsBool(CanHearPlayerKey, false);
        }
        else
        {
            // 놓쳤을 때
            // 기존 타겟이 있었다면 지움
            if (BB->GetValueAsObject(TargetPlayerKey) != nullptr)
            {
                BB->SetValueAsObject(TargetPlayerKey, nullptr);
                UE_LOG(LogTemp, Warning, TEXT("AI: Player LOST."));
            }
        }
    }
    // 2. [청각] 소리를 들었을 때
    else if (SensedClass == UAISense_Hearing::StaticClass())
    {
        // 이미 눈으로 보고 쫓고 있다면 소리는 무시 (시각 우선)
        if (BB->GetValueAsBool(CanSeePlayerKey)) return;

        if (Stimulus.WasSuccessfullySensed())
        {
            BB->SetValueAsBool(CanHearPlayerKey, true);
            BB->SetValueAsVector(LastHeardLocationKey, Stimulus.StimulusLocation);

            UE_LOG(LogTemp, Warning, TEXT("AI: NOISE Heard at %s"), *Stimulus.StimulusLocation.ToString());

            // 약한 참조(Weak Pointer)를 사용하여 안전하게 타이머 실행
            TWeakObjectPtr<ABossAIController> WeakThis(this);

            FTimerHandle TimerHandle;
            GetWorld()->GetTimerManager().SetTimer(TimerHandle, [WeakThis]()
                {
                    // 1. 컨트롤러가 살아있는지 확인 (죽었으면 실행 안 함)
                    if (ABossAIController* StrongThis = WeakThis.Get())
                    {
                        // 2. 블랙보드가 살아있는지 확인
                        if (StrongThis->BB)
                        {
                            StrongThis->BB->SetValueAsBool(CanHearPlayerKey, false);
                        }
                    }
                }, 5.0f, false);
        }
    }
}
