#pragma once

#include "CoreMinimal.h"
#include "MainClass/Objects/Doors/DoorBase.h"
#include "ShortLockerDoor.generated.h"

/**
 * 
 */
UCLASS()
class ESCAPETHEOVERTIMEVR_API AShortLockerDoor : public ADoorBase
{
	GENERATED_BODY()
	
public:
	AShortLockerDoor();

protected:
	virtual void BeginPlay() override;

	virtual void OpenDoor() override;
};
