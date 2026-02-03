#pragma once

#include "CoreMinimal.h"
#include "MainClass/Objects/Props/PropBase.h"
#include "Sound/SoundCue.h" 
#include "KeycardReader.generated.h"

UCLASS()
class ESCAPETHEOVERTIMEVR_API AKeycardReader : public APropBase
{
	GENERATED_BODY()

public:
	AKeycardReader();

protected:
	virtual void ActivateProp(AActor* Activator) override;

	virtual void Grab_Implementation(USceneComponent* HandController) override;

	virtual void Release_Implementation(FVector ThrowVelocity) override;

private:

	UPROPERTY(EditAnywhere, Category = "Audio")
	USoundCue* AccessGrantedSound;
};
