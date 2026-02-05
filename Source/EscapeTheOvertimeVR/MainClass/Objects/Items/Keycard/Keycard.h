#pragma once

#include "CoreMinimal.h"
#include "MainClass/Objects/Items/ItemBase.h"
#include "Keycard.generated.h"

/**
 * 
 */
UCLASS()
class ESCAPETHEOVERTIMEVR_API AKeycard : public AItemBase
{
	GENERATED_BODY()
	
protected:

	AKeycard();

	virtual void ActivateItem(AActor* Activator) override;

	virtual void Grab_Implementation(USceneComponent* HandController) override;

	virtual void Release_Implementation(FVector ThrowVelocity) override;

public:

	bool bIsKeycardUsed = false;

};
