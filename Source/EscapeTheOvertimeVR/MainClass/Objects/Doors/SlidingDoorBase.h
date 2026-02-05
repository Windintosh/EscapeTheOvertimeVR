#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "Components/BoxComponent.h"
#include "Components/StaticMeshComponent.h"
#include "Components/TimelineComponent.h"
#include "Kismet/GameplayStatics.h"
#include "VRGrabInterface.h"
#include "Sound/SoundCue.h" 
#include "SlidingDoorBase.generated.h"

UCLASS()
class ESCAPETHEOVERTIMEVR_API ASlidingDoorBase : public AActor, public IVRGrabInterface
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	ASlidingDoorBase();

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Components")
	UBoxComponent* BoxComp;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Components")
	UStaticMeshComponent* MeshComp;

	UPROPERTY(EditAnywhere, Category = "Animation")
	UTimelineComponent* SlidingTimeline;

	UPROPERTY(EditAnywhere, Category = "Animation")
	UCurveFloat* SlidingDoorCurve;

	float InitialMoveValue; //value to change when moving

	float MovableValue; //how much to move

	float TargetMoveValue; //target value to move

	bool bIsOpen = false;

	UFUNCTION()
	virtual void OnOverlapBegin(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult);

	UFUNCTION()
	virtual void UpdateSDMovement(float Value);

	// override 키워드 유지 (부모의 함수를 재정의한다는 뜻)
	UFUNCTION()
	virtual void Grab_Implementation(USceneComponent* HandController) override;
	
	UFUNCTION()
	virtual void Release_Implementation(FVector ThrowVelocity) override;

	void OpenDoor();

	virtual void CloseDoor();

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Door")
	bool bIsUnopenable = false;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Door")
	bool bIsUnclosable = false;

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

	UPROPERTY(EditAnywhere, Category = "Sound")
	USoundCue* CloseSound;

	UPROPERTY(EditAnywhere, Category = "Audio")
	USoundCue* OpenSound;

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

};
