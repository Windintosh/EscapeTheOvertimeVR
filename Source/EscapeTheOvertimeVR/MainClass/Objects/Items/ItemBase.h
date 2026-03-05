#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "ItemInterface.h"
#include "Components/SphereComponent.h"
#include "VRGrabInterface.h"
#include "Sound/SoundCue.h"
#include "ItemBase.generated.h"

UCLASS()
class ESCAPETHEOVERTIMEVR_API AItemBase : public AActor, public IItemInterface, public IVRGrabInterface
{
	GENERATED_BODY()

public:
	// Sets default values for this actor's properties
	AItemBase();

	//returns the type of the item
	virtual FName GetItemType() const override;

	virtual int32 GetItemIndex() const override;

	virtual void DestroyItem();

	virtual void Grab_Implementation(USceneComponent* HandController) override;

	virtual void Release_Implementation(FVector ThrowVelocity) override;

	virtual void OnAction_Implementation(float Value) override;

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Item")
	FName ItemType;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Item")
	int32 ItemIndex;

	// --- [UI 표시용 데이터 추가] ---
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Item UI")
	FText ItemDisplayName; // 아이템 이름 (예: 보안 카드키)

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Item UI")
	FText ItemDescription; // 아이템 설명 (예: 엘리베이터 권한 획득)

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Item UI")
	UTexture2D* ItemIcon;  // UI 아이콘 텍스처
	// ---------------------------

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Item")
	USceneComponent* Scene;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Item")
	USphereComponent* Collision;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Item")
	UStaticMeshComponent* StaticMesh;


	virtual void OnItemOverlap(UPrimitiveComponent* OverlappedComp, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult);

	virtual void OnItemEndOverlap(UPrimitiveComponent* OverlappedComp, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex);

	//when Item is used
	virtual void ActivateItem(AActor* Activator) override;

	UPROPERTY(EditAnywhere, Category = "Interaction")
	bool bIsGrabbable = true;

	UPROPERTY(EditAnywhere, Category = "Interaction")
	bool bIsGimmickItem = false;

	UPROPERTY(EditAnywhere, Category = "Interaction")
	USoundCue* ItemSound;

public:
	// Called every frame
	virtual void Tick(float DeltaTime) override;

};
