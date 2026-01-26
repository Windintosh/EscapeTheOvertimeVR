#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "SlidingDoorBase.h"
#include "Components/BoxComponent.h"
#include "Components/StaticMeshComponent.h"
#include "Components/TimelineComponent.h"
#include "Kismet/GameplayStatics.h"
#include "ItemSpawner.h"
#include "CabinetDoor.generated.h"

UCLASS()
class ESCAPETHEOVERTIMEVR_API ACabinetDoor : public ASlidingDoorBase
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	ACabinetDoor();

protected:	
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

	virtual void UpdateSDMovement(float Value) override;

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	//AItemSpawner* ItemSpawner;

};
