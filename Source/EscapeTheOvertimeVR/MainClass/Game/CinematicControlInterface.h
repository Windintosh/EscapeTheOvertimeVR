#pragma once

#include "CoreMinimal.h"
#include "UObject/Interface.h"
#include "CinematicControlInterface.generated.h"

class ALevelSequenceActor;
class ACineCameraActor;

UINTERFACE(BlueprintType)
class ESCAPETHEOVERTIMEVR_API UCinematicControlInterface : public UInterface
{
	GENERATED_BODY()
};

/**
 * 10개 이상의 시네마틱 분기를 처리하기 위한 공용 인터페이스
 */
class ESCAPETHEOVERTIMEVR_API ICinematicControlInterface
{
	GENERATED_BODY()

public:
	// 시퀀서 액터와 카메라 액터를 동시에 전달받도록 시그니처 고정
	UFUNCTION(BlueprintCallable, BlueprintNativeEvent, Category = "Cinematic")
	void StartCinematic(ALevelSequenceActor* SequenceActor, ACineCameraActor* CameraActor);

	UFUNCTION(BlueprintCallable, BlueprintNativeEvent, Category = "Cinematic")
	void EndCinematic();
};