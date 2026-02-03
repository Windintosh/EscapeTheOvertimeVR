#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "Components/SphereComponent.h"
#include "Components/BoxComponent.h"
#include "VRGrabInterface.h"
#include "PropBase.generated.h"

//basically this is no-index, no-type version of Item
UCLASS()
class ESCAPETHEOVERTIMEVR_API APropBase : public AActor, public IVRGrabInterface
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	APropBase();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Item")
	USceneComponent* Scene;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Item")
	UBoxComponent* Collision;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Item")
	UStaticMeshComponent* StaticMesh;

	UFUNCTION()
	virtual void OnPropOverlap(UPrimitiveComponent* OverlappedComp, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult);

	UFUNCTION()
	virtual void OnPropEndOverlap(UPrimitiveComponent* OverlappedComp, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex);

	virtual void ActivateProp(AActor* Activator);

	virtual void DestroyProp();

	// override 키워드 유지 (부모의 함수를 재정의한다는 뜻)
	UFUNCTION()
	virtual void Grab_Implementation(USceneComponent* HandController) override;

	UFUNCTION()
	virtual void Release_Implementation(FVector ThrowVelocity) override;

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

};
