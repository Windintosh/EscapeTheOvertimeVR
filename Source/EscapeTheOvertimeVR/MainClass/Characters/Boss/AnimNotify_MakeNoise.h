#pragma once

#include "CoreMinimal.h"
#include "Animation/AnimNotifies/AnimNotify.h"
#include "AnimNotify_MakeNoise.generated.h"

/**
 * 애니메이션에서 이 지점을 지날 때 AI에게 소음 신호를 보냄
 */
UCLASS()
class ESCAPETHEOVERTIMEVR_API UAnimNotify_MakeNoise : public UAnimNotify
{
	GENERATED_BODY()

public:
	UAnimNotify_MakeNoise();

	// 소리의 크기 (1.0 = 보통, 높을수록 AI가 더 잘 들음)
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "AI")
	float Loudness = 1.0f;

	// 소리가 도달하는 최대 거리 (0이면 자동 계산)
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "AI")
	float MaxRange = 0.0f;

	// 노티파이 이름 커스텀 (에디터 표시용)
	virtual FString GetNotifyName_Implementation() const override;

	// 실제 실행 로직
	virtual void Notify(USkeletalMeshComponent* MeshComp, UAnimSequenceBase* Animation, const FAnimNotifyEventReference& EventReference) override;
};
