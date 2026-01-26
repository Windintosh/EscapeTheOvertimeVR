#include "MainClass/Objects/Doors/SinkDoors/SinkDrawer.h"

ASinkDrawer::ASinkDrawer()
{
	BoxComp->SetBoxExtent(FVector(33.f, 35.f, 15.5f)); //make it half! 59 70 31
	MeshComp->SetRelativeLocation(FVector(20.f, -58.f, -31.f), false);
	//static ConstructorHelpers::FObjectFinder<UStaticMesh> CabinetMesh(TEXT("/Game/EscapeTheOvertime/00_Maps/UseAsset/Sink_Cabinet/scene_003.scene_003"));
	//if (CabinetMesh.Succeeded())
	//{
	//	MeshComp->SetStaticMesh(CabinetMesh.Object);
	//}
	//else
	//{
	//	UE_LOG(LogTemp, Error, TEXT("Failed to get StaticMesh for %s"), *GetName());
	//}

	//static ConstructorHelpers::FObjectFinder<UMaterial> CabinetMaterial(TEXT("/Game/EscapeTheOvertime/00_Maps/UseAsset/File_Cabinets_02-Freepoly_org/21___Default.21___Default"));
	//if (CabinetMaterial.Succeeded())
	//{
	//	MeshComp->GetStaticMesh()->SetMaterial(0, CabinetMaterial.Object);
	//}
	//else
	//{
	//	UE_LOG(LogTemp, Error, TEXT("Failed to get Material for %s"), *GetName());
	//}
	MeshComp->SetupAttachment(RootComponent);
	MovableValue = 40.f;
}

void ASinkDrawer::BeginPlay()
{
	Super::BeginPlay();

	InitialMoveValue = MeshComp->GetRelativeLocation().X;
	TargetMoveValue = InitialMoveValue - MovableValue;
}

void ASinkDrawer::UpdateSDMovement(float Value)
{
	float NewLocation = FMath::Lerp(InitialMoveValue, TargetMoveValue, Value);
	float InitialYValue = MeshComp->GetRelativeLocation().Y;
	float InitialZValue = MeshComp->GetRelativeLocation().Z;
	MeshComp->SetRelativeLocation(FVector(NewLocation, InitialYValue, InitialZValue), false);
}
