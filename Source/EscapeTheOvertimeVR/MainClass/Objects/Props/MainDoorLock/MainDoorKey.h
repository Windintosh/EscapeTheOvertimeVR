#pragma once

#include "CoreMinimal.h"
#include "MainClass/Objects/Props/PropBase.h"
#include "MainDoorKey.generated.h"

/**
 * 
 */
UCLASS()
class ESCAPETHEOVERTIMEVR_API AMainDoorKey : public APropBase
{
	GENERATED_BODY()
	
public:
	AMainDoorKey();

protected:	
	virtual void BeginPlay() override;	

	virtual void OnPropOverlap(UPrimitiveComponent* OverlappedComp, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult) override;

public:
	virtual void Tick(float DeltaTime) override;

	virtual void ActivateProp(AActor* Activator) override;
};
