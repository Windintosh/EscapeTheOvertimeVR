#pragma once

#include "CoreMinimal.h"
//#include "GameFramework/GameState.h"
#include "GameFramework/GameStateBase.h"
#include "ItemSpawner.h"
#include "ETOGameState.generated.h"

/**
 * 
 */
UCLASS()
class ESCAPETHEOVERTIMEVR_API AETOGameState : public AGameStateBase
{
	GENERATED_BODY()
	
public:
	virtual void BeginPlay() override;

	void SpawnRandomItems();

	const int32 MinItemIndex = 0; //Keycard
	const int32 MaxItemIndex = 6; //MuffleItem, SpeedUpItem, DistractionItem, HealItem, MaxHPUpItem, TQAmmo

	bool bIsKeycardSpawned = false;

	UFUNCTION()
	void LoadPlayerHP();

	UFUNCTION()
	void SavePlayerHP();

	UFUNCTION()
	void SaveTime();

	UFUNCTION(BlueprintCallable)
	void LoadTime();

	UFUNCTION()
	void SaveItems();

	UFUNCTION()
	void LoadItems();

	UFUNCTION()
	void SavePlayerGotGolden();

	UFUNCTION()
	void LoadPlayerGotGolden();

	UFUNCTION()
	void SaveStuffs();

	UFUNCTION()
	void LoadStuffs();
};
