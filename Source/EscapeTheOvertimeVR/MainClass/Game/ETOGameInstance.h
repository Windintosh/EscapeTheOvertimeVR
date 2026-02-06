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

	float CurrentTimeMinute = 0.f;

	int32 CurrentTimeHour = 20;

	float SavedElapsedTime = 0.f;

	int32 DistractionItemCount = 3;

	int32 TQAmmoCount = 3;

public:
	UFUNCTION(BlueprintCallable, Category = "Player Data")
	void SavePlayerHP(float HP);

	UFUNCTION(BlueprintCallable, Category = "Player Data")
	float LoadPlayerHP();

	void SaveTime(float ElapsedTime, int32 Hour, float Minute);

	float LoadElapsedTime();

	int32 LoadTimeHour();

	float LoadTimeMinute();

	void SaveItemCounts(int32 DI, int32 TQA);

	int32 LoadDistractionItemCount();

	int32 LoadTQAmmoCount();

	bool bPlayerGotGolden = false;

	void SaveGotGolden(bool bGot);

	bool LoadGotGolden();
};
