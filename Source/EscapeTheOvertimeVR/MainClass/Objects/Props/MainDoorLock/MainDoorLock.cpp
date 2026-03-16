#include "MainClass/Objects/Props/MainDoorLock/MainDoorLock.h"
#include "MainDoorKey.h"
#include "MainDoor.h"

AMainDoorLock::AMainDoorLock()
{
	PrimaryActorTick.bCanEverTick = false;
	bIsGimmickProp = true;
	bIsGrabbable = false;
	Collision->SetCollisionResponseToChannel(ECC_Pawn, ECR_Ignore);
}

void AMainDoorLock::BeginPlay()
{
	Super::BeginPlay();
}

void AMainDoorLock::OnPropOverlap(UPrimitiveComponent* OverlappedComp, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{
	AMainDoorKey* MainDoorKey = Cast<AMainDoorKey>(OtherActor);

	//play sound or effect here

	if(MainDoorKey)
	{
		UE_LOG(LogTemp, Warning, TEXT("Main Door Lock activated by Main Door Key!"));
		UnlockDoors();
		MainDoorKey->ActivateProp(this);
		DestroyProp();
	}
}

void AMainDoorLock::UnlockDoors()
{
	TArray<AActor*> FoundDoors;
	UGameplayStatics::GetAllActorsOfClass(GetWorld(), AMainDoor::StaticClass(), FoundDoors);
	for (AActor* DoorActor : FoundDoors)
	{
		AMainDoor* MainDoor = Cast<AMainDoor>(DoorActor);
		if (MainDoor)
		{
			MainDoor->bIsUnopenable = false;
		}
	}
}

void AMainDoorLock::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
}
