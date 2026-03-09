// Copyright Epic Games, Inc. All Rights Reserved.

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
	/** * 시네마틱을 시작합니다. 이제 인풋은 항상 허용되며 시점만 카메라에 고정됩니다.
	 */
	UFUNCTION(BlueprintCallable, BlueprintNativeEvent, Category = "Cinematic")
	void StartCinematic(ALevelSequenceActor* SequenceActor, ACineCameraActor* CameraActor);

	UFUNCTION(BlueprintCallable, BlueprintNativeEvent, Category = "Cinematic")
	void EndCinematic();
};