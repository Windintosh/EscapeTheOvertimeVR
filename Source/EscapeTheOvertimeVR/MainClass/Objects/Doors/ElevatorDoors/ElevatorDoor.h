#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "SlidingDoorBase.h"
#include "Components/BoxComponent.h"
#include "Components/StaticMeshComponent.h"
#include "Components/TimelineComponent.h"
#include "Kismet/GameplayStatics.h"
#include "Sound/SoundCue.h" 
#include "ElevatorDoor.generated.h"

UCLASS()
class ESCAPETHEOVERTIMEVR_API AElevatorDoor : public ASlidingDoorBase
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	AElevatorDoor();

	void OpenDoor();
	
	void CloseDoor();

	void CloseDoorOnTimer();
protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

	virtual void UpdateSDMovement(float Value) override;

	virtual void OnOverlapBegin(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult) override;

	/*
	UPROPERTY(EditAnywhere, Category = "Sound")
	USoundCue* CloseSound;

	UPROPERTY(EditAnywhere, Category = "Audio")
	USoundCue* OpenSound;
	*/

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

};
