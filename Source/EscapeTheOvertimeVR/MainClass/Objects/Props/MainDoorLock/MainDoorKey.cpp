#include "MainClass/Objects/Props/MainDoorLock/MainDoorKey.h"

AMainDoorKey::AMainDoorKey()
{
	PrimaryActorTick.bCanEverTick = false;
	bIsGimmickProp = true;
}

void AMainDoorKey::BeginPlay()
{
	Super::BeginPlay();
}

void AMainDoorKey::OnPropOverlap(UPrimitiveComponent* OverlappedComp, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{

}

void AMainDoorKey::ActivateProp(AActor* Activator)
{
	DestroyProp();
}

void AMainDoorKey::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);	
}
