#pragma once

#include "CoreMinimal.h"
#include "MainClass/Objects/Items/ItemBase.h"
#include "GeometryCollection/GeometryCollection.h"
#include "GeometryCollection/GeometryCollectionComponent.h"
#include "GameFramework/ProjectileMovementComponent.h"
#include "ThrownItem.generated.h"

/**
 * 
 */
UINTERFACE(MinimalAPI, Blueprintable)
class UBossHit : public UInterface
{
	GENERATED_BODY()
};

class IBossHit
{
	GENERATED_BODY()

public:
	UFUNCTION(BlueprintNativeEvent, BlueprintCallable, Category = "Interaction")
	void Interact(AActor* Interactor);
};


UCLASS()
class ESCAPETHEOVERTIMEVR_API AThrownItem : public AItemBase
{
	GENERATED_BODY()
	
protected:

	AThrownItem();

	UFUNCTION()
	virtual void OnItemOverlap(UPrimitiveComponent* OverlappedComp, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult) override;

	UFUNCTION()
	virtual void OnItemHit(UPrimitiveComponent* HitComp, AActor* OtherActor, UPrimitiveComponent* OtherComp, FVector NormalImpulse, const FHitResult& Hit);

	virtual void ActivateItem(AActor* Activator) override;

	virtual void BeginPlay() override;

	//UFUNCTION()
	//void OnProjectileStop(const FHitResult& ImpactResult);

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Components")
	UGeometryCollectionComponent* GeometryCollectionComponent;

	UPROPERTY(EditAnywhere, Category = "Destruction")
	UGeometryCollection* RestCollectionAsset;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Components")
	UProjectileMovementComponent* ProjectileMovement;

	UPROPERTY(EditDefaultsOnly, Category = "Effects")
	TSubclassOf<AActor> BreakerClass;

	// 타이머를 관리할 핸들
	FTimerHandle CollisionTimerHandle;

	// 타이머가 끝나면 호출될 함수
	UFUNCTION()
	void DisablePawnCollision();

	bool bIsBroken = false;

public:
	virtual void OnConstruction(const FTransform& Transform) override;
};
