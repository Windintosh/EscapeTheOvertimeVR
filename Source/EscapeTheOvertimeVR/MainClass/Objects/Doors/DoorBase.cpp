#include "MainClass/Objects/Doors/DoorBase.h"
#include "EscapeTheOvertimeCharacter.h"

// Sets default values
ADoorBase::ADoorBase()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	BoxComp = CreateDefaultSubobject<UBoxComponent>(TEXT("Box Component"));
	SetRootComponent(BoxComp); // RootComponent = BoxComp;
	BoxComp->SetCollisionProfileName(TEXT("OverlapAllDynamic"));
	BoxComp->SetBoxExtent(FVector(30.f, 74.f, 147.f)); //y,z /2
	BoxComp->SetRelativeLocation(FVector(0.f, 0.f, 147.f), false);
	BoxComp->SetCanEverAffectNavigation(false);

	MeshComp = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("Static Mesh"));
	MeshComp->SetupAttachment(BoxComp);
	MeshComp->SetRelativeLocation(FVector(0.f, 74.f, -147.f), false);
	MeshComp->SetCanEverAffectNavigation(false);

	DoorTimeline = CreateDefaultSubobject<UTimelineComponent>(TEXT("DoorTimeline"));

	MovableYaw = 90.f;
}

// Called when the game starts or when spawned
void ADoorBase::BeginPlay()
{
	Super::BeginPlay();

	InitialYaw = MeshComp->GetRelativeRotation().Yaw;
	TargetYaw = InitialYaw + MovableYaw; // need to add L/R logic later

	BoxComp->OnComponentBeginOverlap.AddDynamic(this, &ADoorBase::OnOverlapBegin);

	if (DoorCurve)
	{
		FOnTimelineFloat TimelineCallback;
		TimelineCallback.BindUFunction(this, FName("UpdateDoorMovement"));
		DoorTimeline->AddInterpFloat(DoorCurve, TimelineCallback);
	}
	else
	{
		UE_LOG(LogTemp, Error, TEXT("%s No DoorCurve Found! Assign in Blueprint!"), *GetName());
	}
}

// Called every frame
void ADoorBase::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

void ADoorBase::Grab_Implementation(USceneComponent* HandController)
{
	if(!bIsOpen)
		OpenDoor();
	else 
		CloseDoor();
}

void ADoorBase::Release_Implementation(FVector ThrowVelocity)
{

}

void ADoorBase::OpenDoor()
{
	if (bIsOpen) return;
	if (bIsUnopenable) return;
	DoorTimeline->PlayFromStart();
	//UpdateDoorMovement(1.0f);
	if (OpenSound)
	{
		UGameplayStatics::PlaySoundAtLocation(this, OpenSound, GetActorLocation());
	}
	bIsOpen = true;
}

void ADoorBase::CloseDoor()
{
	if (!bIsOpen) return;
	if (bIsUnclosable) return;
	DoorTimeline->ReverseFromEnd();
	if (CloseSound)
	{
		UGameplayStatics::PlaySoundAtLocation(this, CloseSound, GetActorLocation());
	}
	bIsOpen = false;
}

void ADoorBase::OnOverlapBegin(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult) 
{
	AEscapeTheOvertimeCharacter* PlayerCharacter = Cast<AEscapeTheOvertimeCharacter>(OtherActor);
	if (PlayerCharacter && !bIsOpen)
	{
		//will timeline work?
		//UE_LOG(LogTemp, Warning, TEXT("Opening Door"));
		//OpenDoor();
	}
}

void ADoorBase::UpdateDoorMovement(float Value)
{
	FRotator NewRotation = MeshComp->GetRelativeRotation();
	NewRotation.Yaw = FMath::Lerp(InitialYaw, TargetYaw, Value);
	MeshComp->SetRelativeRotation(NewRotation);
}

//flag for the file change