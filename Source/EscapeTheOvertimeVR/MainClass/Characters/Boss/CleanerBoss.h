#pragma once

#include "CoreMinimal.h"
#include "MainClass/Characters/Boss/Boss.h" 
#include "CleanerBoss.generated.h"

UCLASS()
class ESCAPETHEOVERTIMEVR_API ACleanerBoss : public ABoss
{
	GENERATED_BODY()

public:
	virtual void BeginPlay() override;

	// 청소할 때 재생할 애니메이션 몽타주
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "AI")
	UAnimMontage* CleaningMontage;

	// 엘리베이터 앞 위치 (원위치)
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "AI")
	FVector HomeLocation;

	// 닦을 때 바라볼 방향
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "AI")
	FRotator HomeRotation;

};
