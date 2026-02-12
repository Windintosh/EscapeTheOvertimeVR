#pragma once

#include "CoreMinimal.h"
#include "MainClass/Objects/Doors/DoorBase.h"
#include "TallLockerDoor.generated.h"

/**
 * 
 */
UCLASS()
class ESCAPETHEOVERTIMEVR_API ATallLockerDoor : public ADoorBase
{
	GENERATED_BODY()
	
public:
	ATallLockerDoor();

	virtual void BeginPlay() override;

	virtual void OpenDoor() override;
};
