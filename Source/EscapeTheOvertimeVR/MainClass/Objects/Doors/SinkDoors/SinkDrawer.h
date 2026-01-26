#pragma once

#include "CoreMinimal.h"
#include "MainClass/Objects/Doors/SlidingDoorBase.h"
#include "SinkDrawer.generated.h"

/**
 * 
 */
UCLASS()
class ESCAPETHEOVERTIMEVR_API ASinkDrawer : public ASlidingDoorBase
{
	GENERATED_BODY()
	
public:
	ASinkDrawer();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

	virtual void UpdateSDMovement(float Value) override;

};
