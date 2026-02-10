#pragma once

#include "CoreMinimal.h"
#include "MainClass/Objects/Doors/DoorBase.h"
#include "RestroomDoor.generated.h"

/**
 * 
 */
UCLASS()
class ESCAPETHEOVERTIMEVR_API ARestroomDoor : public ADoorBase
{
	GENERATED_BODY()

public:

	ARestroomDoor();

	UFUNCTION(BlueprintCallable, Category = "Door")
	void OpenDoorFromOthers();

protected:

	virtual void BeginPlay() override;

	
};
