#include "MainClass/Objects/Props/PropBase.h"
#include "EscapeTheOvertimeCharacter.h"
#include "Kismet/GameplayStatics.h"

// Sets default values
APropBase::APropBase()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	Scene = CreateDefaultSubobject<USceneComponent>(TEXT("Scene"));
	SetRootComponent(Scene);

	Collision = CreateDefaultSubobject<USphereComponent>(TEXT("Collision"));
	Collision->SetCollisionProfileName(TEXT("OverlapAllDynamic"));

	Collision->SetupAttachment(Scene);

	StaticMesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("StaticMesh"));
	StaticMesh->SetupAttachment(Collision);

	Collision->InitSphereRadius(20.0f);
	Collision->OnComponentBeginOverlap.AddDynamic(this, &APropBase::OnPropOverlap);
	Collision->OnComponentEndOverlap.AddDynamic(this, &APropBase::OnPropEndOverlap);

}

// Called when the game starts or when spawned
void APropBase::BeginPlay()
{
	Super::BeginPlay();
	
}

void APropBase::OnPropOverlap(UPrimitiveComponent* OverlappedComp, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{
	AEscapeTheOvertimeCharacter* PlayerCharacter = Cast<AEscapeTheOvertimeCharacter>(OtherActor);
	if (!PlayerCharacter) return;

	UE_LOG(LogTemp, Warning, TEXT("Player just touched an prop %s"), *GetName());
	ActivateProp(OtherActor);
}

void APropBase::OnPropEndOverlap(UPrimitiveComponent* OverlappedComp, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex)
{

}

void APropBase::ActivateProp(AActor* Activator)
{

}

void APropBase::DestroyProp()
{
	Destroy();
}

// Called every frame
void APropBase::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

