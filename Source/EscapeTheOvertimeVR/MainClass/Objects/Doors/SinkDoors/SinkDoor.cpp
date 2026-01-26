#include "MainClass/Objects/Doors/SinkDoors/SinkDoor.h"

ASinkDoor::ASinkDoor()
{
	BoxComp->SetBoxExtent(FVector(7.f, 28.5f, 48.5f)); // 7 57 97
	BoxComp->SetRelativeLocation(FVector(0.f, 0.f, 48.5f), false);
	MeshComp->SetRelativeLocation(FVector(0.f, 28.f, -48.5f), false);

	//static ConstructorHelpers::FObjectFinder<UStaticMesh> CabinetMesh(TEXT("/Game/EscapeTheOvertime/00_Maps/UseAsset/Sink_Cabinet/SinkDoor.SinkDoor"));
	//if (CabinetMesh.Succeeded())
	//{
	//	MeshComp->SetStaticMesh(CabinetMesh.Object);
	//}
	//else
	//{
	//	UE_LOG(LogTemp, Error, TEXT("Failed to get StaticMesh for %s"), *GetName());
	//}

	//static ConstructorHelpers::FObjectFinder<UMaterial> CabinetMaterial(TEXT("/Game/EscapeTheOvertime/00_Maps/UseAsset/Sink_Cabinet/Wood13-basecolor.Wood13-basecolor"));
	//if (CabinetMaterial.Succeeded())
	//{
	//	MeshComp->GetStaticMesh()->SetMaterial(0, CabinetMaterial.Object);
	//}
	//else
	//{
	//	UE_LOG(LogTemp, Error, TEXT("Failed to get Material for %s"), *GetName());
	//}
	MeshComp->SetupAttachment(RootComponent);
}

void ASinkDoor::BeginPlay()
{
	Super::BeginPlay();
	TargetYaw = InitialYaw - MovableYaw;
}
