#pragma once

#include "CoreMinimal.h"
#include "MainClass/Characters/Boss/Boss.h"
#include "MainClass/Characters/Creatures/MonkeyBanana.h"
#include "Monkey.generated.h"

/**
 * 
 */
UCLASS()
class ESCAPETHEOVERTIMEVR_API AMonkey : public ABoss
{
	GENERATED_BODY()

public:
	void ThrowBanana();

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Projectile")
	TSubclassOf<class AMonkeyBanana> BananaClass;
};
