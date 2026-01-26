#include "MainClass/Characters/Boss/EnvQueryTest_SearchPriority.h"
#include "SearchPointComponent.h"

UEnvQueryTest_SearchPriority::UEnvQueryTest_SearchPriority()
{
	TestPurpose = EEnvTestPurpose::Score; //the higher, the better
	ScoringEquation = EEnvTestScoreEquation::Linear;
}

void UEnvQueryTest_SearchPriority::RunTest(FEnvQueryInstance& QueryInstance) const
{
    // EQS가 찾아낸 아이템들(책상, 복사기 등)을 순회
    for (FEnvQueryInstance::ItemIterator It(this, QueryInstance); It; ++It)
    {
        AActor* ItemActor = GetItemActor(QueryInstance, It.GetIndex());
        if (!ItemActor) continue;

        // 액터에서 컴포넌트 찾기
        USearchPointComponent* SearchComp = ItemActor->FindComponentByClass<USearchPointComponent>();
        if (SearchComp)
        {
            // Enum 값을 float 점수로 변환 (High=3, Medium=2, Low=1)
            float Score = (float)SearchComp->Priority;
            It.SetScore(TestPurpose, FilterType, Score, 0.f, 10.f);
        }
    }
}
