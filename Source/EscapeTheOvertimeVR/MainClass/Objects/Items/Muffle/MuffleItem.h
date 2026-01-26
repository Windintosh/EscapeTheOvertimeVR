#pragma once

#include "CoreMinimal.h"
#include "MainClass/Objects/Items/ItemBase.h"
#include "MuffleItem.generated.h"

/**
 * 
 */
UCLASS()
class ESCAPETHEOVERTIMEVR_API AMuffleItem : public AItemBase
{
	GENERATED_BODY()
	
protected:
	AMuffleItem();

	virtual void ActivateItem(AActor* Activator) override;
};
