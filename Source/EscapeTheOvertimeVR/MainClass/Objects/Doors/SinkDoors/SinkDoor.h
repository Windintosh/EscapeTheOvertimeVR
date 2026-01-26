#pragma once

#include "CoreMinimal.h"
#include "MainClass/Objects/Doors/DoorBase.h"
#include "SinkDoor.generated.h"

/**
 * 
 */
UCLASS()
class ESCAPETHEOVERTIMEVR_API ASinkDoor : public ADoorBase
{
	GENERATED_BODY()
	
public:
	ASinkDoor();

protected:

	virtual void BeginPlay() override;
};
