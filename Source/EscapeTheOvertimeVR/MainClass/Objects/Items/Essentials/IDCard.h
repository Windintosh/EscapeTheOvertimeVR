#pragma once

#include "CoreMinimal.h"
#include "MainClass/Objects/Items/ItemBase.h"
#include "IDCard.generated.h"

/**
 * 
 */
UCLASS()
class ESCAPETHEOVERTIMEVR_API AIDCard : public AItemBase
{
	GENERATED_BODY()
	
public:
	AIDCard();

	virtual void ActivateItem(AActor* Activator) override;
};
