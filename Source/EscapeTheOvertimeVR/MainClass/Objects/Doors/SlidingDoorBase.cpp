#include "MainClass/Objects/Doors/SlidingDoorBase.h"
#include "EscapeTheOvertimeCharacter.h"


// Sets default values
ASlidingDoorBase::ASlidingDoorBase()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;
	BoxComp = CreateDefaultSubobject<UBoxComponent>(TEXT("Box Component"));
	SetRootComponent(BoxComp); // RootComponent = BoxComp;
	BoxComp->SetCollisionProfileName(TEXT("OverlapAllDynamic"));

	MeshComp = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("Static Mesh"));
	MeshComp->SetupAttachment(BoxComp);

	SlidingTimeline = CreateDefaultSubobject<UTimelineComponent>(TEXT("SlidingTimeline"));

	//set movablevalue here;
}

void ASlidingDoorBase::UpdateSDMovement(float Value)
{
	//need to override, set initialmovevalue here

}

// Called when the game starts or when spawned
void ASlidingDoorBase::BeginPlay()
{
	Super::BeginPlay();
	
	BoxComp->OnComponentBeginOverlap.AddDynamic(this, &ASlidingDoorBase::OnOverlapBegin);

	//InitialMoveValue = MeshComp->GetRelativeLocation().X;//override this and MovableMoveValue;
	//TargetMoveValue = InitialMoveValue + MovableValue; //->set those in BeginPlay

	if (SlidingDoorCurve)
	{
		FOnTimelineFloat TimelineCallback;
		TimelineCallback.BindUFunction(this, FName("UpdateSDMovement"));
		SlidingTimeline->AddInterpFloat(SlidingDoorCurve, TimelineCallback);
	}
	else
	{
		UE_LOG(LogTemp, Error, TEXT("%s No DoorCurve Found! Assign in Blueprint!"), *GetName());
	}
}

// Called every frame
void ASlidingDoorBase::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

void ASlidingDoorBase::OnOverlapBegin(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{
	AEscapeTheOvertimeCharacter* PlayerCharacter = Cast<AEscapeTheOvertimeCharacter>(OtherActor);
	if (PlayerCharacter && !bIsOpen)
	{
		//timeline failed-> fix it later
		SlidingTimeline->PlayFromStart();
		UE_LOG(LogTemp, Warning, TEXT("Character is near SlidingDoor"));
		//UpdateCabMovement(1.0f);
		bIsOpen = true;
	}
}
