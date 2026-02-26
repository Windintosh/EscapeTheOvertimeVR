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

class ESCAPETHEOVERTIMEVR_API ICinematicControlInterface
{
	GENERATED_BODY()

public:
	/** * @param bAllowInput 체크 시 시네마틱 도중에도 이동/상호작용 인풋을 허용합니다.
	 */
	UFUNCTION(BlueprintCallable, BlueprintNativeEvent, Category = "Cinematic")
	void StartCinematic(ALevelSequenceActor* SequenceActor, ACineCameraActor* CameraActor, bool bAllowInput);

	UFUNCTION(BlueprintCallable, BlueprintNativeEvent, Category = "Cinematic")
	void EndCinematic();
};