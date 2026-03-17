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

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Player Data")
	float PlayerMaxHP = 100.0f;

	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = "Player Data")
	float CurrentTimeMinute = 0.f;

	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = "Player Data")
	int32 CurrentTimeHour = 20;

	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = "Player Data")
	float SavedElapsedTime = 0.f;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Player Data")
	int32 DistractionItemCount = 3;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Player Data")
	int32 TQAmmoCount = 3;

	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = "Player Data")
	bool bPlayerGotTQ = false;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Player Data")
	bool bGotPill = false;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Player Data")
	bool bPlayerGotGolden = false;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Player Data")
	bool bGotIDCard = false;

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

	void SaveGotGolden(bool bGot);

	bool LoadGotGolden();

	void SaveGotTQ(bool bGot);

	bool LoadGotTQ();

	void SaveGotPill(bool bGot);

	bool LoadGotPill();

	void SaveGotIDCard(bool bGot);

	bool LoadGotIDCard();
};
