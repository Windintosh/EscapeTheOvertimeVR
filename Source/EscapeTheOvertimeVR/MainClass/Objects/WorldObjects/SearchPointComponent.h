#pragma once

#include "CoreMinimal.h"
#include "Components/SceneComponent.h"
#include "SearchPointComponent.generated.h"

UENUM(BlueprintType)
enum class ESearchPriority : uint8
{
	None        UMETA(DisplayName = "None"),
	Low         UMETA(DisplayName = "Low (Copier)"),
	Medium      UMETA(DisplayName = "Medium (Partition)"),
	High        UMETA(DisplayName = "High (Under Desk)")
};

UCLASS( ClassGroup=(Custom), meta=(BlueprintSpawnableComponent) )
class ESCAPETHEOVERTIMEVR_API USearchPointComponent : public USceneComponent
{
	GENERATED_BODY()

public:	
	// Sets default values for this component's properties
	USearchPointComponent();

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "AI") 
	ESearchPriority Priority; // Add this in editor

	bool bIsSearched = false; 

protected:
	// Called when the game starts
	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction) override;

		
};
