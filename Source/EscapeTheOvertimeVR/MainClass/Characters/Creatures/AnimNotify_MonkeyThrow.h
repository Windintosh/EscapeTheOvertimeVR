#pragma once

#include "CoreMinimal.h"
#include "Animation/AnimNotifies/AnimNotify.h"
#include "MonkeyBanana.h"
#include "AnimNotify_MonkeyThrow.generated.h"

/**
 * 
 */
UCLASS()
class ESCAPETHEOVERTIMEVR_API UAnimNotify_MonkeyThrow : public UAnimNotify
{
	GENERATED_BODY()
	
public:

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Projectile")
	TSubclassOf<class AMonkeyBanana> BananaClass;

protected:
	virtual void Notify(USkeletalMeshComponent* MeshComp, UAnimSequenceBase* Animation) override;
};
