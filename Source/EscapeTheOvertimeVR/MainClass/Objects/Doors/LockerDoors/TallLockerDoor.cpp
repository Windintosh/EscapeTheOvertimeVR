#include "MainClass/Objects/Doors/LockerDoors/TallLockerDoor.h"

ATallLockerDoor::ATallLockerDoor()
{
	BoxComp->SetBoxExtent(FVector(2.f, 21.f, 117.f));
	BoxComp->SetRelativeLocation(FVector(0.f, 0.f, 117.f), false);
	MeshComp->SetRelativeLocation(FVector(0.f, -20.5f, -116.f), false);
	//static ConstructorHelpers::FObjectFinder<UStaticMesh> TallLockerMesh(TEXT("/Game/EscapeTheOvertime/00_Maps/UseAsset/Cabinet/OfficeCabinetDoor4.OfficeCabinetDoor4"));
	//if (TallLockerMesh.Succeeded())
	//{
	//	MeshComp->SetStaticMesh(TallLockerMesh.Object);
	//}
	//else
	//{
	//	UE_LOG(LogTemp, Error, TEXT("Failed to get StaticMesh for %s"), *GetName());
	//}

	//static ConstructorHelpers::FObjectFinder<UMaterial> TallLockerMaterial(TEXT("/Game/EscapeTheOvertime/00_Maps/UseAsset/Cabinet/Material_001.Material_001"));
	//if (TallLockerMaterial.Succeeded())
	//{
	//	MeshComp->GetStaticMesh()->SetMaterial(0, TallLockerMaterial.Object);
	//}
	//else
	//{
	//	UE_LOG(LogTemp, Error, TEXT("Failed to get Material0 for %s"), *GetName());
	//}
	//static ConstructorHelpers::FObjectFinder<UMaterial> TallLockerMaterial2(TEXT("/Game/EscapeTheOvertime/00_Maps/UseAsset/Cabinet/Material_002.Material_002"));
	//if (TallLockerMaterial2.Succeeded())
	//{
	//	MeshComp->GetStaticMesh()->SetMaterial(1, TallLockerMaterial2.Object);
	//}
	//else
	//{
	//	UE_LOG(LogTemp, Error, TEXT("Failed to get Material1 for %s"), *GetName());
	//}
	MeshComp->SetupAttachment(RootComponent);
}

void ATallLockerDoor::BeginPlay()
{
	Super::BeginPlay(); //4 41 234
}

void ATallLockerDoor::OpenDoor()
{
	Super::OpenDoor();

	bIsUnclosable = true;
}
