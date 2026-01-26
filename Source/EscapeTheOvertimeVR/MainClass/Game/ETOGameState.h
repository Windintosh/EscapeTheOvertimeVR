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
	const int32 MaxItemIndex = 5; //MuffleItem, SpeedUpItem, DistractionItem, HealItem, MaxHPUpItem

	bool bIsKeycardSpawned = false;

	void LoadPlayerHP();

	void SavePlayerHP();
};
