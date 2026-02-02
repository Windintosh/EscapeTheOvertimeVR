#pragma once

#include "CoreMinimal.h"
#include "MainClass/Objects/Items/ItemBase.h"
#include "GeometryCollection/GeometryCollection.h"
#include "GeometryCollection/GeometryCollectionComponent.h"
#include "VRGrabInterface.h"
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

	virtual void Tick(float DeltaTime) override;

	//UFUNCTION()
	//void OnProjectileStop(const FHitResult& ImpactResult);

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Components")
	UGeometryCollectionComponent* GeometryCollectionComponent;

	UPROPERTY(EditAnywhere, Category = "Destruction")
	UGeometryCollection* RestCollectionAsset;

	UPROPERTY(EditDefaultsOnly, Category = "Effects")
	TSubclassOf<AActor> BreakerClass;

	// 타이머를 관리할 핸들
	FTimerHandle CollisionTimerHandle;
	FTimerHandle IgnorePawnTimerHandle; // 던진 후 충돌 복구용 타이머 추가

	// 타이머가 끝나면 호출될 함수
	UFUNCTION()
	void DisablePawnCollision();
	// 던진 후 Pawn과 충돌 다시 켜기
	UFUNCTION()
	void ReEnablePawnCollision();

	bool bIsBroken = false;

	bool bWasThrown = false;

public:
	virtual void OnConstruction(const FTransform& Transform) override;

	// VR Grab Interface 구현
	UFUNCTION()
	virtual void Grab_Implementation(USceneComponent* HandController) override;

	UFUNCTION()
	virtual void Release_Implementation(FVector ThrowVelocity) override;

	// 던질 때 속도 보정을 위한 변수 (선택사항)
	UPROPERTY(EditDefaultsOnly, Category = "VR Physics")
	float ThrowPowerMultiplier = 1.0f; // 너무 약하게 던져지면 값을 올리세요
};
