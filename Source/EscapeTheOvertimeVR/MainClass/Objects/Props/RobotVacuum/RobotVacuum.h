#pragma once

#include "CoreMinimal.h"
#include "MainClass/Objects/Props/PropBase.h"
#include "RobotVacuum.generated.h"

/**
 * 
 */
UCLASS()
class ESCAPETHEOVERTIMEVR_API ARobotVacuum : public APropBase
{
	GENERATED_BODY()

public:
	ARobotVacuum();
	
protected:

	virtual void BeginPlay() override;

	virtual void Tick(float DeltaTime) override;

	virtual void Grab_Implementation(USceneComponent* HandController) override;

	virtual void Release_Implementation(FVector ThrowVelocity) override;

	virtual void OnPropOverlap(UPrimitiveComponent* OverlappedComp, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult) override;

	virtual void ActivateProp(AActor* Activator) override;

	UFUNCTION(BlueprintCallable, Category = "Robot Logic")
	void StartRVMovement();

	UFUNCTION(BlueprintCallable, Category = "Robot Logic")
	void StopRVMovement();

private:
	// 다음 목표 지점을 결정하는 함수
	void PickNextTarget();

	// 이동 가능한 상태인지 확인하는 플래그
	bool bCanMove;

	// 현재 목표 위치
	FVector TargetLocation;

	// 이동 속도 (에디터에서 수정 가능)
	UPROPERTY(EditAnywhere, Category = "Robot Settings")
	float MoveSpeed = 300.0f;

	// 목표 도달 허용 오차 (이 거리 안에 들어오면 도착으로 간주)
	float AcceptanceRadius = 10.0f;

	// 랜덤 이동 반경
	UPROPERTY(EditAnywhere, Category = "Robot Settings")
	float RandomMoveRadius = 1000.0f;

};
