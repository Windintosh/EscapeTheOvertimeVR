#pragma once

#include "CoreMinimal.h"
#include "MainClass/Objects/Doors/DoorBase.h"
#include "AltarDoor.generated.h"

/**
 * 
 */
UCLASS()
class ESCAPETHEOVERTIMEVR_API AAltarDoor : public ADoorBase
{
	GENERATED_BODY()
	
public:
	AAltarDoor();

protected:
	virtual void BeginPlay() override;

public:
	virtual void OpenDoor() override;
};
