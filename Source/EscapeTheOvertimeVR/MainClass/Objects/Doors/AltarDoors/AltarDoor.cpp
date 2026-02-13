#include "MainClass/Objects/Doors/AltarDoors/AltarDoor.h"

AAltarDoor::AAltarDoor()
{
	BoxComp->SetBoxExtent(FVector(42.f, 98.5f, 353.f)); // 42 197 706; y,z /2
	BoxComp->SetRelativeLocation(FVector(21.f, 98.5, 353.f), false);
	MeshComp->SetRelativeLocation(FVector(-21.f, 98.5f, -353.f), false);
	bIsUnopenable = true;

	bIsUnclosable = true;
}

void AAltarDoor::BeginPlay()
{
	Super::BeginPlay();

	if (GetClass()->GetName().Contains("Left"))
	{
		TargetYaw = InitialYaw - MovableYaw;
	}
}

void AAltarDoor::OpenDoor()
{
	Super::OpenDoor();

	BoxComp->SetCollisionEnabled(ECollisionEnabled::NoCollision);
}
