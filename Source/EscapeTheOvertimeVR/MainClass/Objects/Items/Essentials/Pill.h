#pragma once

#include "CoreMinimal.h"
#include "MainClass/Objects/Items/ItemBase.h"
#include "Pill.generated.h"

/**
 * 
 */
UCLASS()
class ESCAPETHEOVERTIMEVR_API APill : public AItemBase
{
	GENERATED_BODY()

public:
	APill();

protected:
	virtual void ActivateItem(AActor* Activator) override;

};
