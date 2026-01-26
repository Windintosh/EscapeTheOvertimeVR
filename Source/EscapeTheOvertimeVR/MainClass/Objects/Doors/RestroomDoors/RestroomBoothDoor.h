#pragma once

#include "CoreMinimal.h"
#include "MainClass/Objects/Doors/DoorBase.h"
#include "RestroomBoothDoor.generated.h"

/**
 * 
 */
UCLASS()
class ESCAPETHEOVERTIMEVR_API ARestroomBoothDoor : public ADoorBase
{
	GENERATED_BODY()

public:
	ARestroomBoothDoor();

protected:

	virtual void BeginPlay() override;
	
};
