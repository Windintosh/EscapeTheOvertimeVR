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



};
