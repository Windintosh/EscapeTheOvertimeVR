#include "MainClass/Game/ETOGameState.h"
#include "Kismet/GameplayStatics.h"
#include "Kismet/KismetMathLibrary.h"
#include "ETOGameInstance.h"
#include "HorrorCharacter.h"
#include "VRCharacterPawn.h"
#include "HorrorPlayerController.h"
#include "Engine/OverlapResult.h"

void AETOGameState::BeginPlay()
{
	Super::BeginPlay();

	LoadStuffs();
	FString CurrentMapName = UGameplayStatics::GetCurrentLevelName(GetWorld(), true);
	if (CurrentMapName == "FirstMap") SpawnRandomItems();

	UGameplayStatics::SetGamePaused(GetWorld(), false);
}

void AETOGameState::SpawnRandomItems()
{
	//Need to add conditions, perhaps, for checking the map itself(name or sth)
		
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
		if (index == ItemSpots.Num() - 1) 
		{
			/* //if no keycard has spawned, do it again
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
			*/
			int32 NewRandIndex = FMath::RandRange(0, ItemSpots.Num() - 1);
			AItemSpawner* NewSpawnPoint = Cast<AItemSpawner>(ItemSpots[NewRandIndex]);
			if (NewSpawnPoint)
			{
				FVector StartLocation = NewSpawnPoint->GetActorLocation();
				FCollisionShape Sphere = FCollisionShape::MakeSphere(50.f);
				FCollisionQueryParams QueryParams;
				QueryParams.AddIgnoredActor(NewSpawnPoint);

				TArray<FOverlapResult> OverlapResults;
				bool bHasOverlap = GetWorld()->OverlapMultiByChannel(
					OverlapResults,
					StartLocation,
					FQuat::Identity,
					ECC_WorldDynamic, //channel of items
					Sphere,
					QueryParams
				);
				if (bHasOverlap)
				{
					for (const FOverlapResult& Result : OverlapResults)
					{
						AActor* HitActor = Result.GetActor();
						if (HitActor && HitActor->IsA(AItemBase::StaticClass()))
						{
							UE_LOG(LogTemp, Display, TEXT("Destroying overlapping item: %s"), *HitActor->GetName());
							HitActor->Destroy();
						}
					}
				}

				AActor* SpawnedActor = NewSpawnPoint->SpawnItem(0); //force spawn keycard
				if (SpawnedActor)
				{
					UE_LOG(LogTemp, Display, TEXT("Keycard Forced Spawned at point %d"), NewRandIndex);
					bIsKeycardSpawned = true;
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

void AETOGameState::SaveItems()
{
	UETOGameInstance* ETOGI = Cast<UETOGameInstance>(GetWorld()->GetGameInstance());
	AHorrorCharacter* PlayerCharacter = Cast<AHorrorCharacter>(GetWorld()->GetFirstPlayerController()->GetCharacter());
	if (ETOGI && PlayerCharacter)
	{
		ETOGI->SaveItemCounts(PlayerCharacter->DistractionItemQuantity, PlayerCharacter->TQAmmoQuantity); //Make functions?
		UE_LOG(LogTemp, Warning, TEXT("Items are saved. DI: %d, TQA: %d"), PlayerCharacter->DistractionItemQuantity, PlayerCharacter->TQAmmoQuantity);
	}
}

void AETOGameState::LoadItems()
{
	UETOGameInstance* ETOGI = Cast<UETOGameInstance>(GetWorld()->GetGameInstance());
	AHorrorCharacter* PlayerCharacter = Cast<AHorrorCharacter>(GetWorld()->GetFirstPlayerController()->GetCharacter());
	if (ETOGI && PlayerCharacter)
	{
		PlayerCharacter->DistractionItemQuantity = ETOGI->LoadDistractionItemCount();
		PlayerCharacter->TQAmmoQuantity = ETOGI->LoadTQAmmoCount();
		UE_LOG(LogTemp, Warning, TEXT("Items are loaded. DI: %d, TQA: %d"), PlayerCharacter->DistractionItemQuantity, PlayerCharacter->TQAmmoQuantity);
	}
}

void AETOGameState::SavePlayerGotGolden()
{
	UETOGameInstance* ETOGI = Cast<UETOGameInstance>(GetWorld()->GetGameInstance());
	AVRCharacterPawn* PlayerCharacter = Cast<AVRCharacterPawn>(GetWorld()->GetFirstPlayerController()->GetCharacter());
	if (ETOGI && PlayerCharacter)
	{
		ETOGI->SaveGotGolden(PlayerCharacter->bGotGolden);
		//UE_LOG(LogTemp, Warning, TEXT("Player Got Golden is saved: %s"), PlayerCharacter->bGotGolden);
	}
}

void AETOGameState::LoadPlayerGotGolden()
{
	UETOGameInstance* ETOGI = Cast<UETOGameInstance>(GetWorld()->GetGameInstance());
	AVRCharacterPawn* PlayerCharacter = Cast<AVRCharacterPawn>(GetWorld()->GetFirstPlayerController()->GetCharacter());
	if (ETOGI && PlayerCharacter)
	{
		PlayerCharacter->bGotGolden = ETOGI->LoadGotGolden();
		//UE_LOG(LogTemp, Warning, TEXT("Player Got Golden is loaded: %s"), PlayerCharacter->bGotGolden);
	}
}

void AETOGameState::SavePlayerGotTQ()
{
	UETOGameInstance* ETOGI = Cast<UETOGameInstance>(GetWorld()->GetGameInstance());
	AVRCharacterPawn* PlayerCharacter = Cast<AVRCharacterPawn>(GetWorld()->GetFirstPlayerController()->GetCharacter());
	if (ETOGI && PlayerCharacter)
	{
		ETOGI->SaveGotTQ(PlayerCharacter->bHasTranquilizer);
		//UE_LOG(LogTemp, Warning, TEXT("Player Got Golden is saved: %s"), PlayerCharacter->bGotGolden);
	}
}

void AETOGameState::LoadPlayerGotTQ()
{
	UETOGameInstance* ETOGI = Cast<UETOGameInstance>(GetWorld()->GetGameInstance());
	AVRCharacterPawn* PlayerCharacter = Cast<AVRCharacterPawn>(GetWorld()->GetFirstPlayerController()->GetCharacter());
	if (ETOGI && PlayerCharacter)
	{
		PlayerCharacter->bHasTranquilizer = ETOGI->LoadGotTQ();
		//UE_LOG(LogTemp, Warning, TEXT("Player Got Golden is loaded: %s"), PlayerCharacter->bGotGolden);
	}
}

void AETOGameState::SaveStuffs()
{
	SavePlayerHP();
	SaveTime();
	SaveItems();
	SavePlayerGotGolden();
	SaveGotPill();
	SaveGotIDCard();
	SavePlayerGotTQ();
}

void AETOGameState::LoadStuffs()
{
	LoadPlayerHP();
	LoadTime();
	LoadItems();
	LoadPlayerGotGolden();
	LoadGotPill();
	LoadGotIDCard();
	LoadPlayerGotTQ();
}

void AETOGameState::SaveGotPill()
{
	UETOGameInstance* ETOGI = Cast<UETOGameInstance>(GetWorld()->GetGameInstance());
	AVRCharacterPawn* PlayerCharacter = Cast<AVRCharacterPawn>(GetWorld()->GetFirstPlayerController()->GetCharacter());
	if (ETOGI && PlayerCharacter)
	{
		ETOGI->SaveGotPill(PlayerCharacter->bGotPill);
		//UE_LOG(LogTemp, Warning, TEXT("Player Got Golden is saved: %s"), PlayerCharacter->bGotGolden);
	}
}

void AETOGameState::LoadGotPill()
{
	UETOGameInstance* ETOGI = Cast<UETOGameInstance>(GetWorld()->GetGameInstance());
	AVRCharacterPawn* PlayerCharacter = Cast<AVRCharacterPawn>(GetWorld()->GetFirstPlayerController()->GetCharacter());
	if (ETOGI && PlayerCharacter)
	{
		PlayerCharacter->bGotPill = ETOGI->LoadGotPill();
		//UE_LOG(LogTemp, Warning, TEXT("Player Got Golden is loaded: %s"), PlayerCharacter->bGotGolden);
	}
}

void AETOGameState::SaveGotIDCard()
{
	UETOGameInstance* ETOGI = Cast<UETOGameInstance>(GetWorld()->GetGameInstance());
	AVRCharacterPawn* PlayerCharacter = Cast<AVRCharacterPawn>(GetWorld()->GetFirstPlayerController()->GetCharacter());
	if (ETOGI && PlayerCharacter)
	{
		ETOGI->SaveGotIDCard(PlayerCharacter->bGotIDCard);
		//UE_LOG(LogTemp, Warning, TEXT("Player Got Golden is saved: %s"), PlayerCharacter->bGotGolden);
	}
}

void AETOGameState::LoadGotIDCard()
{
	UETOGameInstance* ETOGI = Cast<UETOGameInstance>(GetWorld()->GetGameInstance());
	AVRCharacterPawn* PlayerCharacter = Cast<AVRCharacterPawn>(GetWorld()->GetFirstPlayerController()->GetCharacter());
	if (ETOGI && PlayerCharacter)
	{
		PlayerCharacter->bGotIDCard = ETOGI->LoadGotIDCard();
		//UE_LOG(LogTemp, Warning, TEXT("Player Got Golden is loaded: %s"), PlayerCharacter->bGotGolden);
	}
}
