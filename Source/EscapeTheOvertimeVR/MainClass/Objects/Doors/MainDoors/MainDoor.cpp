#include "MainClass/Objects/Doors/MainDoors/MainDoor.h"

AMainDoor::AMainDoor()
{
	//need to add logic to change rotation->Done!


}

void AMainDoor::BeginPlay()
{
	Super::BeginPlay();

	if (GetClass()->GetName().Contains("Left"))
	{
		TargetYaw = InitialYaw - MovableYaw;
	}

}
