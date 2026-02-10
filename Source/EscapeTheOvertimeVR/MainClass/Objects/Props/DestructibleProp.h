#pragma once

#include "CoreMinimal.h"
#include "MainClass/Objects/Props/PropBase.h"
#include "DestructibleProp.generated.h"

/**
 * 
 */
UCLASS()
class ESCAPETHEOVERTIMEVR_API ADestructibleProp : public APropBase
{
	GENERATED_BODY()
	
public:
		ADestructibleProp();

protected:
	virtual void BeginPlay() override;

	virtual void OnPropOverlap(UPrimitiveComponent* OverlappedComp, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult) override;

	virtual void ActivateProp(AActor* Activator) override;

	virtual void Tick(float DeltaTime) override;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Destructible Prop")
	bool bHasHealth = true;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Destructible Prop")
	float Health = 100.f;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Destructible Prop")
	int32 HitCount = 5;

	virtual float TakeDamage(float DamageAmount, struct FDamageEvent const& DamageEvent, class AController* EventInstigator, AActor* DamageCauser) override;

	void DestructProp();

	bool bIsDestroyed = false;
};
