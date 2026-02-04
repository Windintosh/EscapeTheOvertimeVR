#pragma once

#include "CoreMinimal.h"
#include "Engine/GameInstance.h"
#include "ETOGameInstance.generated.h"

/**
 * 
 */
UCLASS()
class ESCAPETHEOVERTIMEVR_API UETOGameInstance : public UGameInstance
{
	GENERATED_BODY()
	
public:
	UETOGameInstance();

protected:

	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = "Player Data")
	float PlayerMaxHP = 100.0f;

	float CurrentTimeMinute;

	int32 CurrentTimeHour;

	float SavedElapsedTime;

public:
	UFUNCTION(BlueprintCallable, Category = "Player Data")
	void SavePlayerHP(float HP);

	UFUNCTION(BlueprintCallable, Category = "Player Data")
	float LoadPlayerHP();

	void SaveTime(float ElapsedTime, int32 Hour, float Minute);

	float LoadElapsedTime();

	int32 LoadTimeHour();

	float LoadTimeMinute();
};
