#pragma once

#include "CoreMinimal.h"
#include "MainClass/Objects/Items/ItemBase.h"
#include "DistractionItem.generated.h"

/**
 * 
 */
UCLASS()
class ESCAPETHEOVERTIMEVR_API ADistractionItem : public AItemBase
{
	GENERATED_BODY()
	
protected:

	ADistractionItem();

	virtual void ActivateItem(AActor* Activator) override;

};
