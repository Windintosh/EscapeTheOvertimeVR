#pragma once

#include "CoreMinimal.h"
#include "Boss.h"
#include "Dancer.generated.h"

class UNavModifierComponent;

UCLASS()
class ESCAPETHEOVERTIMEVR_API ADancer : public ABoss
{
	GENERATED_BODY()

public:
	// Sets default values for this character's properties
	ADancer();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "AI")
	UNavModifierComponent* NavModifierComp;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "AI")
	FVector NavModifierExtentSize;

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	// Called to bind functionality to input
	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;

};
