#include "MainClass/Game/ETOGameState.h"
#include "Kismet/GameplayStatics.h"
#include "Kismet/KismetMathLibrary.h"
#include "ETOGameInstance.h"
#include "HorrorCharacter.h"
#include "HorrorPlayerController.h"

void AETOGameState::BeginPlay()
{
	Super::BeginPlay();

	LoadPlayerHP();
	SpawnRandomItems();

	UGameplayStatics::SetGamePaused(GetWorld(), false);
}

void AETOGameState::SpawnRandomItems()
{
	UGameplayStatics::SetGamePaused(GetWorld(), true);
	UE_LOG(LogTemp, Warning, TEXT("Game Paused for Item Spawn"));
	TArray<AActor*> ItemSpots;
	UGameplayStatics::GetAllActorsOfClass(GetWorld(), AItemSpawner::StaticClass(), ItemSpots);
	UE_LOG(LogTemp, Display, TEXT("%d Itemspots Spawned"), ItemSpots.Num());
	//for (AActor* ItemSpot : ItemSpots)
	//{
	//	int32 RandIndex = FMath::RandRange(MinItemIndex, MaxItemIndex);
	//	AItemSpawner* SpawnPoint = Cast<AItemSpawner>(ItemSpot);
	//	if (SpawnPoint)
	//	{
	//		SpawnPoint->SpawnItem(RandIndex);
	//		if (SpawnPoint->ItemIndex == 0)
	//		{
	//			break;
	//		}
	//	}
	//}
	int32 index = 0;
	int32 loopcount = 0;
	for (index = 0; index < ItemSpots.Num(); index++)
	{
		int32 RandIndex = FMath::RandRange(MinItemIndex, MaxItemIndex);
		AItemSpawner* SpawnPoint = Cast<AItemSpawner>(ItemSpots[index]);
		loopcount++;
		if (SpawnPoint)
		{
			AActor* SpawnedActor = SpawnPoint->SpawnItem(RandIndex);
			if (SpawnedActor)
			{
				UE_LOG(LogTemp, Display, TEXT("%s Spawned(Index: %d) at point %d"), *SpawnPoint->ItemType.ToString(), SpawnPoint->ItemIndex, index);
				if (SpawnPoint->ItemIndex == 0) //if keycard, break the loop
				{
					bIsKeycardSpawned = true;
					UE_LOG(LogTemp, Display, TEXT("Keycard Spawned; Finishing first loop"));
					break;
				}
				//else if (SpawnPoint->ItemType.IsNone())
				//{
				//	UE_LOG(LogTemp, Display, TEXT("ItemSpawner Spawned nothing! retrying..."));
				//	SpawnPoint->SpawnItem(RandIndex); //need to change
				//}
			}
			else
			{
				UE_LOG(LogTemp, Warning, TEXT("Ghost Spawner Found at Location: %s"), *SpawnPoint->GetActorLocation().ToString());
				UE_LOG(LogTemp, Warning, TEXT("Spawn Failed at point %d. Ignoring this spawner."), index);
			}
		}
		if (index == ItemSpots.Num() - 1) //if no keycard has spawned, do it again
		{
			index = -1;
			loopcount = 0;
			UE_LOG(LogTemp, Display, TEXT("No Keycard; retrying"));
			TArray<AActor*> DestroyedItems;
			UGameplayStatics::GetAllActorsOfClass(GetWorld(), AItemBase::StaticClass(), DestroyedItems);
			for (AActor* DItem : DestroyedItems)
			{
				AItemBase* NoMore = Cast<AItemBase>(DItem);
				if (NoMore)
				{	
					NoMore->DestroyItem();
				}
			}
		}
	}
	for (loopcount; loopcount < ItemSpots.Num(); loopcount++) //spawn for the rest of the loop
	{
		int32 RandIndex = FMath::RandRange(MinItemIndex+1, MaxItemIndex); //no keycard
		AItemSpawner* SpawnPoint = Cast<AItemSpawner>(ItemSpots[loopcount]);
		if (SpawnPoint)
		{
			AActor* SpawnedActor = SpawnPoint->SpawnItem(RandIndex);
			if (SpawnedActor) 
			{
				UE_LOG(LogTemp, Display, TEXT("%s Spawned(Index: %d) at point %d"), *SpawnPoint->ItemType.ToString(), SpawnPoint->ItemIndex, loopcount);
			}
		}
	}
	UE_LOG(LogTemp, Warning, TEXT("Item Spawn Completed, Unpausing Game"));
	UGameplayStatics::SetGamePaused(GetWorld(), false);
}

void AETOGameState::LoadPlayerHP()
{
	UETOGameInstance* ETOGI = Cast<UETOGameInstance>(GetWorld()->GetGameInstance());
	AHorrorCharacter* PlayerCharacter = Cast<AHorrorCharacter>(GetWorld()->GetFirstPlayerController()->GetCharacter());

	if (ETOGI && PlayerCharacter)
	{
		PlayerCharacter->SetHP(ETOGI->LoadPlayerHP());
		UE_LOG(LogTemp, Warning, TEXT("Player MaxHP is set to %.1f -> %.1f"), ETOGI->LoadPlayerHP(), PlayerCharacter->GetCurrentHP());
	}
}

void AETOGameState::SavePlayerHP()
{
	UETOGameInstance* ETOGI = Cast<UETOGameInstance>(GetWorld()->GetGameInstance());
	AHorrorCharacter* PlayerCharacter = Cast<AHorrorCharacter>(GetWorld()->GetFirstPlayerController()->GetCharacter());

	if (ETOGI && PlayerCharacter)
	{
		ETOGI->SavePlayerHP(PlayerCharacter->GetMaxHP());
		UE_LOG(LogTemp, Warning, TEXT("Player MaxHP is saved %.1f -> %.1f"),PlayerCharacter->GetMaxHP(), ETOGI->LoadPlayerHP());
	}
}

void AETOGameState::SaveTime()
{
	UETOGameInstance* ETOGI = Cast<UETOGameInstance>(GetWorld()->GetGameInstance());
	AHorrorPlayerController* PC = Cast<AHorrorPlayerController>(GetWorld()->GetFirstPlayerController());
	if (ETOGI && PC)
	{
		ETOGI->SaveTime(PC->ElapsedTime, PC->CurrentHour, PC->CurrentMinute);
		UE_LOG(LogTemp, Warning, TEXT("Time is saved. ET: %f, Hour: %d, Minute: %.1f"), PC->ElapsedTime, PC->CurrentHour, PC->CurrentMinute);
	}
}

void AETOGameState::LoadTime()
{
	UETOGameInstance* ETOGI = Cast<UETOGameInstance>(GetWorld()->GetGameInstance());
	AHorrorPlayerController* PC = Cast<AHorrorPlayerController>(GetWorld()->GetFirstPlayerController());
	if (ETOGI && PC)
	{
		PC->ElapsedTime = ETOGI->LoadElapsedTime();
		PC->CurrentHour = ETOGI->LoadTimeHour();
		PC->CurrentMinute = ETOGI->LoadTimeMinute();
		UE_LOG(LogTemp, Warning, TEXT("Time is loaded. ET: %f, Hour: %d, Minute: %.1f"), PC->ElapsedTime, PC->CurrentHour, PC->CurrentMinute);
	}
}
