#pragma once

#include "CoreMinimal.h"
#include "MainClass/Objects/Items/ItemBase.h"
#include "HealItem.generated.h"

/**
 * 
 */
UCLASS()
class ESCAPETHEOVERTIMEVR_API AHealItem : public AItemBase
{
	GENERATED_BODY()
	
protected:
	AHealItem();

	virtual void ActivateItem(AActor* Activator) override;

	float HealAmount = 50.f;
};
