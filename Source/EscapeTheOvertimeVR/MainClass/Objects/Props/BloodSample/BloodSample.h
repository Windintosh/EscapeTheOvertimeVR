#pragma once

#include "CoreMinimal.h"
#include "MainClass/Objects/Props/PropBase.h" // 부모 클래스 헤더 포함
#include "BloodSample.generated.h"

/**
 * 맥거핀 아이템: 눈알이 든 혈액 샘플 실린더
 * PropBase를 상속받아 VR 인터랙션 기능을 그대로 사용합니다.
 */
UCLASS()
class ESCAPETHEOVERTIMEVR_API ABloodSample : public APropBase
{
	GENERATED_BODY()

public:
	ABloodSample();

protected:
	virtual void BeginPlay() override;

public:
	virtual void Tick(float DeltaTime) override;

	// --- 컴포넌트 추가 ---

	/** 혈액 액체 메쉬 (실린더 내부를 채우는 용도) */
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Components|Blood")
	UStaticMeshComponent* BloodLiquidMesh;

	/** 실린더 내부에서 떠다닐 눈알 메쉬 */
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Components|Blood")
	UStaticMeshComponent* EyeballMesh;

	// --- 눈알 물리 연출 설정 ---

	/** 눈알이 위(Z+)를 향해 회전하는 속도 */
	UPROPERTY(EditAnywhere, Category = "Blood|Eye Logic")
	float EyeRotationSpeed = 5.0f;

	/** 눈알이 위아래로 일렁이는 진폭 */
	UPROPERTY(EditAnywhere, Category = "Blood|Eye Logic")
	float FloatAmplitude = 1.5f;

	/** 눈알이 위아래로 일렁이는 속도 (주파수) */
	UPROPERTY(EditAnywhere, Category = "Blood|Eye Logic")
	float FloatFrequency = 2.5f;

private:
	FVector InitialEyeLocation;
	float RunningTime = 0.0f;
};