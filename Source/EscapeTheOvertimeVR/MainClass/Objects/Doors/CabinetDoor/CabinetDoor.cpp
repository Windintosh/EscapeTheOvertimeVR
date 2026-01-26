#include "CabinetDoor.h"
#include "UObject/ConstructorHelpers.h"
#include "EscapeTheOvertimeCharacter.h"


// Sets default values
ACabinetDoor::ACabinetDoor()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	//ItemSpawner = CreateDefaultSubobject<AItemSpawner>(TEXT("ItemSpawner"));
	//ItemSpawner->AttachToComponent(MeshComp, FAttachmentTransformRules::SnapToTargetNotIncludingScale);

	BoxComp->SetBoxExtent(FVector(60.f, 60.f, 7.5f)); //make it half!

	//static ConstructorHelpers::FObjectFinder<UStaticMesh> CabinetMesh(TEXT("/Game/EscapeTheOvertime/00_Maps/UseAsset/File_Cabinets_02-Freepoly_org/MiniCabinets_Door5.MiniCabinets_Door5"));
	//if (CabinetMesh.Succeeded())
	//{
	//	MeshComp->SetStaticMesh(CabinetMesh.Object);
	//} 
	//else
	//{
	//	UE_LOG(LogTemp, Error, TEXT("Failed to get StaticMesh for %s"), *GetName());
	//}
	//
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

// Called when the game starts or when spawned
void ACabinetDoor::BeginPlay()
{
	Super::BeginPlay();

	InitialMoveValue = MeshComp->GetRelativeLocation().X;
	TargetMoveValue = InitialMoveValue + MovableValue;
}

void ACabinetDoor::UpdateSDMovement(float Value)
{
	float NewLocation = FMath::Lerp(InitialMoveValue, TargetMoveValue, Value);
	float InitialYValue = MeshComp->GetRelativeLocation().Y;
	float InitialZValue = MeshComp->GetRelativeLocation().Z;
	MeshComp->SetRelativeLocation(FVector(NewLocation, InitialYValue, InitialZValue), false);
}

// Called every frame
void ACabinetDoor::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

