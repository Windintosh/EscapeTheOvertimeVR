#include "MainClass/Objects/Doors/LockerDoors/ShortLockerDoor.h"

AShortLockerDoor::AShortLockerDoor()
{
	BoxComp->SetBoxExtent(FVector(2.f, 21.f, 58.f));
	BoxComp->SetRelativeLocation(FVector(0.f, 0.f, 116.f), false); // 4 37 116
	MeshComp->SetRelativeLocation(FVector(0.f, -18.5f, -58.f), false);
	//static ConstructorHelpers::FObjectFinder<UStaticMesh> ShortLockerMesh(TEXT("/Game/EscapeTheOvertime/00_Maps/UseAsset/Cabinet/OfficeCabinetDoor3.OfficeCabinetDoor3"));
	//if (ShortLockerMesh.Succeeded())
	//{
	//	MeshComp->SetStaticMesh(ShortLockerMesh.Object);
	//}
	//else
	//{
	//	UE_LOG(LogTemp, Error, TEXT("Failed to get StaticMesh for %s"), *GetName());
	//}

	//static ConstructorHelpers::FObjectFinder<UMaterial> ShortLockerMaterial(TEXT("/Game/EscapeTheOvertime/00_Maps/UseAsset/Cabinet/Material_001.Material_001"));
	//if (ShortLockerMaterial.Succeeded())
	//{
	//	MeshComp->GetStaticMesh()->SetMaterial(0, ShortLockerMaterial.Object);
	//}
	//else
	//{
	//	UE_LOG(LogTemp, Error, TEXT("Failed to get Material0 for %s"), *GetName());
	//}
	//static ConstructorHelpers::FObjectFinder<UMaterial> ShortLockerMaterial2(TEXT("/Game/EscapeTheOvertime/00_Maps/UseAsset/Cabinet/Material_002.Material_002"));
	//if (ShortLockerMaterial2.Succeeded())
	//{
	//	MeshComp->GetStaticMesh()->SetMaterial(1, ShortLockerMaterial2.Object);
	//}
	//else
	//{
	//	UE_LOG(LogTemp, Error, TEXT("Failed to get Material1 for %s"), *GetName());
	//}
	MeshComp->SetupAttachment(RootComponent);
}

void AShortLockerDoor::BeginPlay()
{
	Super::BeginPlay();
}

void AShortLockerDoor::OpenDoor()
{
	Super::OpenDoor();
	bIsUnclosable = true;
}
