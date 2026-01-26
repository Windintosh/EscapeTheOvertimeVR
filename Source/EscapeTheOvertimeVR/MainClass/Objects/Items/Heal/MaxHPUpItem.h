#pragma once

#include "CoreMinimal.h"
#include "MainClass/Objects/Items/ItemBase.h"
#include "MaxHPUpItem.generated.h"

/**
 * 
 */
UCLASS()
class ESCAPETHEOVERTIMEVR_API AMaxHPUpItem : public AItemBase
{
	GENERATED_BODY()

protected:
	AMaxHPUpItem();

	virtual void ActivateItem(AActor* Activator) override;

	float HealAmount = 10.f;
};
