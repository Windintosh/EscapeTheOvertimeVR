#pragma once

#include "CoreMinimal.h"
#include "MainClass/Characters/Boss/Boss.h"
#include "DoggyX.generated.h"

/**
 * 
 */
UCLASS()
class ESCAPETHEOVERTIMEVR_API ADoggyX : public ABoss
{
	GENERATED_BODY()
	
public:
	ADoggyX();

protected:
	virtual void BeginPlay() override;

	virtual void Attack() override;

	virtual void OnDeath() override;
};
