#include "MainClass/Objects/WorldObjects/ItemSpawner.h"
#include "Components/BoxComponent.h"
#include "Keycard.h"
#include "MuffleItem.h"
#include "SpeedUpItem.h"
#include "DistractionItem.h"
#include "HealItem.h"
#include "MaxHPUpItem.h"
#include "Kismet/KismetMathLibrary.h"

// Sets default values
AItemSpawner::AItemSpawner()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	Scene = CreateDefaultSubobject<USceneComponent>(TEXT("Scene"));
	SetRootComponent(Scene);


	BoxComp = CreateDefaultSubobject<UBoxComponent>(TEXT("Box Collision"));

	//BoxComp->SetMobility(EComponentMobility::Static);
	//BoxComp->SetBoxExtent(FVector(50.0f, 50.0f, 50.0f));

	BoxComp->SetupAttachment(Scene);
}

// Called when the game starts or when spawned
void AItemSpawner::BeginPlay()
{
	Super::BeginPlay();
	
	//SpawnItem(FMath::RandRange(0, 3)); //need to move to GameInstance or sth
}

// Called every frame
void AItemSpawner::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

AActor* AItemSpawner::SpawnItem(int32 Index)
{
	
	switch(Index)
	{
	case 0:
	{
		AKeycard* SpawnedItem = GetWorld()->SpawnActor<AKeycard>(KeycardClass, GetActorLocation(), GetActorRotation());
		if (SpawnedItem) {
			ItemIndex = SpawnedItem->GetItemIndex();
			ItemType = SpawnedItem->GetItemType();
			SpawnedItem->AttachToComponent(BoxComp, FAttachmentTransformRules::SnapToTargetNotIncludingScale);
			UE_LOG(LogTemp, Display, TEXT("Keycard Spawned"));
			return SpawnedItem;
		}
		else return nullptr;
	}
	case 1:
	{
		AMuffleItem* SpawnedItem = GetWorld()->SpawnActor<AMuffleItem>(MuffleItemClass, GetActorLocation(), GetActorRotation());
		if (SpawnedItem) {
			ItemIndex = SpawnedItem->GetItemIndex();
			ItemType = SpawnedItem->GetItemType();
			SpawnedItem->AttachToComponent(BoxComp, FAttachmentTransformRules::SnapToTargetNotIncludingScale);
			UE_LOG(LogTemp, Display, TEXT("MuffleItem Spawned"));
			return SpawnedItem;
		}
		else return nullptr;
	}
	case 2:
	{
		ASpeedUpItem* SpawnedItem = GetWorld()->SpawnActor<ASpeedUpItem>(SpeedUpItemClass, GetActorLocation(), GetActorRotation());
		if (SpawnedItem) {
			ItemIndex = SpawnedItem->GetItemIndex();
			ItemType = SpawnedItem->GetItemType();
			SpawnedItem->AttachToComponent(BoxComp, FAttachmentTransformRules::SnapToTargetNotIncludingScale);
			UE_LOG(LogTemp, Display, TEXT("SpeedUp Spawned"));
			return SpawnedItem;
		}
		else return nullptr;
	}
	case 3:
	{
		ADistractionItem* SpawnedItem = GetWorld()->SpawnActor<ADistractionItem>(DistractionItemClass, GetActorLocation(), GetActorRotation());
		if (SpawnedItem) {
			ItemIndex = SpawnedItem->GetItemIndex();
			ItemType = SpawnedItem->GetItemType();
			SpawnedItem->AttachToComponent(BoxComp, FAttachmentTransformRules::SnapToTargetNotIncludingScale);
			UE_LOG(LogTemp, Display, TEXT("DistractionItem Spawned"));
			return SpawnedItem;
		}
		else return nullptr;
	}
	case 4:
	{
		AHealItem* SpawnedItem = GetWorld()->SpawnActor<AHealItem>(HealItemClass, GetActorLocation(), GetActorRotation());
		if (SpawnedItem) {
			ItemIndex = SpawnedItem->GetItemIndex();
			ItemType = SpawnedItem->GetItemType();
			SpawnedItem->AttachToComponent(BoxComp, FAttachmentTransformRules::SnapToTargetNotIncludingScale);
			UE_LOG(LogTemp, Display, TEXT("HealItem Spawned"));
			return SpawnedItem;
		}
		else return nullptr;
	}
	case 5: 
	{
		AMaxHPUpItem* SpawnedItem = GetWorld()->SpawnActor<AMaxHPUpItem>(MaxHPUpItemClass, GetActorLocation(), GetActorRotation());
		if (SpawnedItem) {
			ItemIndex = SpawnedItem->GetItemIndex();
			ItemType = SpawnedItem->GetItemType();
			SpawnedItem->AttachToComponent(BoxComp, FAttachmentTransformRules::SnapToTargetNotIncludingScale);
			UE_LOG(LogTemp, Display, TEXT("MaxHPUpItem Spawned"));
			return SpawnedItem;
		}
		else return nullptr;
	}
	default:
		return nullptr;
	}

}

