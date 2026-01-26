#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "WarpBox.generated.h"

class UBoxComponent;
// 블루프린트에서 바인딩할 수 있는 델리게이트 선언
DECLARE_DYNAMIC_MULTICAST_DELEGATE(FOnEndingTriggeredDelegate);


UCLASS()
class ESCAPETHEOVERTIMEVR_API AWarpBox : public AActor
{
	GENERATED_BODY()

public:
	AWarpBox();
	// 금요일 맵에서 이 워프박스가 작동하면 이 이벤트가 호출됩니다.
	UPROPERTY(BlueprintAssignable, Category = "Event")
	FOnEndingTriggeredDelegate OnEndingTriggered;


protected:
	virtual void BeginPlay() override;

	// 충돌 컴포넌트
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Components")
	UBoxComponent* TriggerBox;

	// 오버랩 이벤트 함수
	UFUNCTION()
	void OnOverlapBegin(UPrimitiveComponent* OverlappedComp, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult);

	// 4초 뒤 실행될 실제 이동 함수
	void WarpLevel();

	// 타이머 핸들
	FTimerHandle WarpTimerHandle;

	//에디터에서 직접 다음 레벨 이름을 지정
	UPROPERTY(EditAnywhere, Category = "Warp Settings")
	FName NextLevelName;

	// 딜레이 시간 (기본 4초)
	UPROPERTY(EditAnywhere, Category = "Warp Settings")
	float WarpDelayTime = 4.0f;
};
