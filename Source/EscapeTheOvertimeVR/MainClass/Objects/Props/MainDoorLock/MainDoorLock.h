#pragma once

#include "CoreMinimal.h"
#include "MainClass/Objects/Props/PropBase.h"
#include "MainDoorLock.generated.h"

/**
 * 
 */
UCLASS()
class ESCAPETHEOVERTIMEVR_API AMainDoorLock : public APropBase
{
	GENERATED_BODY()
	
public:
	AMainDoorLock();

protected:
	virtual void BeginPlay() override;	

	virtual void OnPropOverlap(UPrimitiveComponent* OverlappedComp, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult) override;

	void UnlockDoors();
public:
	virtual void Tick(float DeltaTime) override;

};
