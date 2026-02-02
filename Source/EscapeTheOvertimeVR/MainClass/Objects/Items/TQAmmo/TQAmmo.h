#pragma once

#include "CoreMinimal.h"
#include "MainClass/Objects/Items/ItemBase.h"
#include "TQAmmo.generated.h"

/**
 * 
 */
UCLASS()
class ESCAPETHEOVERTIMEVR_API ATQAmmo : public AItemBase
{
	GENERATED_BODY()
	
protected:
	ATQAmmo();

	virtual void ActivateItem(AActor* Activator) override;
};
