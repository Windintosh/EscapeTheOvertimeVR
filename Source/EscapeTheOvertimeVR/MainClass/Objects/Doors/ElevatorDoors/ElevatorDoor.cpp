#include "MainClass/Objects/Doors/ElevatorDoors/ElevatorDoor.h"
#include "EscapeTheOvertimeCharacter.h"

// Sets default values
AElevatorDoor::AElevatorDoor()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;


	static ConstructorHelpers::FObjectFinder<USoundCue> CloseSoundAsset(TEXT("/Game/EscapeTheOvertime/06_Audio/SFX/Environment/DoorOpen/Cue_open_newMap_.Cue_open_newMap_"));
	if (CloseSoundAsset.Succeeded())
	{
		CloseSound = CloseSoundAsset.Object;
	}

	static ConstructorHelpers::FObjectFinder<USoundCue> OpenSoundAsset(TEXT("/Game/EscapeTheOvertime/06_Audio/SFX/Environment/DoorOpen/Cue_open_Elevator.Cue_open_Elevator"));
	if (OpenSoundAsset.Succeeded())
	{
		OpenSound = OpenSoundAsset.Object;
	}


	BoxComp->SetBoxExtent(FVector(70.f, 35.f, 147.f));
	BoxComp->SetRelativeLocation(FVector(0.f, 0.f, 147.f), false);

	MovableValue = 75.f;
}

// Called when the game starts or when spawned
void AElevatorDoor::BeginPlay()
{
	Super::BeginPlay();
	InitialMoveValue = MeshComp->GetRelativeLocation().Y;
	TargetMoveValue = InitialMoveValue + MovableValue;//75
}

void AElevatorDoor::UpdateSDMovement(float Value)
{
	float NewLocation = FMath::Lerp(InitialMoveValue, TargetMoveValue, Value);
	float InitialXValue = MeshComp->GetRelativeLocation().X;
	float InitialZValue = MeshComp->GetRelativeLocation().Z;
	MeshComp->SetRelativeLocation(FVector(InitialXValue, NewLocation, InitialZValue), false);
}

void AElevatorDoor::OnOverlapBegin(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{
	//no function on overlap, opens when player interacts with card reader
}

void AElevatorDoor::OpenDoor()
{
	if (bIsOpen) return;
	SlidingTimeline->PlayFromStart();
	if (OpenSound)
	{
		UGameplayStatics::PlaySoundAtLocation(this, OpenSound, GetActorLocation());
	}
	bIsOpen = true;
}

void AElevatorDoor::CloseDoor()
{
	if (!bIsOpen) return;
	SlidingTimeline->ReverseFromEnd();
	if (CloseSound)
	{
		UGameplayStatics::PlaySoundAtLocation(this, CloseSound, GetActorLocation());
	}
	bIsOpen = false;
}

void AElevatorDoor::CloseDoorOnTimer()
{
	FTimerHandle DoorTimerHandle;
	GetWorldTimerManager().SetTimer(DoorTimerHandle, this, &AElevatorDoor::CloseDoor, 2.0f, false);
}

// Called every frame
void AElevatorDoor::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

