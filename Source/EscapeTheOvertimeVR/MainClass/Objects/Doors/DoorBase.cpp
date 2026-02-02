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
	BoxComp->SetBoxExtent(FVector(30.f, 74.f, 147.f));
	BoxComp->SetRelativeLocation(FVector(0.f, 0.f, 147.f), false);

	MeshComp = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("Static Mesh"));
	MeshComp->SetupAttachment(BoxComp);
	MeshComp->SetRelativeLocation(FVector(0.f, 74.f, -147.f), false);

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

void ADoorBase::OnOverlapBegin(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult) 
{
	AEscapeTheOvertimeCharacter* PlayerCharacter = Cast<AEscapeTheOvertimeCharacter>(OtherActor);
	if (PlayerCharacter && !bIsOpen)
	{
		//will timeline work?
		UE_LOG(LogTemp, Warning, TEXT("Character is near Door"));
		DoorTimeline->PlayFromStart();
		//UpdateDoorMovement(1.0f);
		bIsOpen = true;
	}
}

void ADoorBase::UpdateDoorMovement(float Value)
{
	FRotator NewRotation = MeshComp->GetRelativeRotation();
	NewRotation.Yaw = FMath::Lerp(InitialYaw, TargetYaw, Value);
	MeshComp->SetRelativeRotation(NewRotation);
}

