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

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Heal")
	float HealAmount = 20.f;
};
