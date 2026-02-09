#include "MainClass/Objects/Props/PropBase.h"
#include "EscapeTheOvertimeCharacter.h"
#include "Kismet/GameplayStatics.h"

// Sets default values
APropBase::APropBase()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	/*Scene = CreateDefaultSubobject<USceneComponent>(TEXT("Scene"));
	SetRootComponent(Scene);*/

	Collision = CreateDefaultSubobject<UBoxComponent>(TEXT("Collision"));
	Collision->SetCollisionProfileName(TEXT("OverlapAllDynamic"));
	SetRootComponent(Collision);
	//Collision->SetupAttachment(Scene);

	StaticMesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("StaticMesh"));
	StaticMesh->SetupAttachment(Collision);

	Collision->InitBoxExtent(FVector(20.f));
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
	//ActivateProp(OtherActor);
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

void APropBase::Grab_Implementation(USceneComponent* HandController)
{
	// 1. 물리 끄기 (손에 붙이기 위해)
	Collision->SetSimulatePhysics(false);

	// 3. 충돌 처리: 잡고 있는 동안은 Pawn과 부딪히지 않게 함 (떨림 방지)
	Collision->SetCollisionResponseToChannel(ECC_Pawn, ECR_Ignore);

	//if (bIsGimmickProp)
	//{
		StaticMesh->SetCollisionEnabled(ECollisionEnabled::NoCollision);
	//}

	// 4. 손에 부착 (SnapToTarget을 쓰면 손 위치로 강제 이동, KeepWorld는 잡은 위치 유지)
	// 여기서는 자연스러운 잡기를 위해 KeepWorld를 추천하지만, 
	// 총처럼 딱 잡히길 원하면 SnapToTarget 사용
	FAttachmentTransformRules AttachmentRules(EAttachmentRule::KeepWorld, EAttachmentRule::KeepWorld, EAttachmentRule::KeepWorld, true);
	AttachToComponent(HandController, AttachmentRules);
}

void APropBase::Release_Implementation(FVector ThrowVelocity)
{
	// 1. 손에서 분리
	DetachFromActor(FDetachmentTransformRules::KeepWorldTransform);

	if(bIsGimmickProp)
	{
		StaticMesh->SetCollisionEnabled(ECollisionEnabled::QueryAndPhysics);
		//StaticMesh->SetSimulatePhysics(true);
	}

	// 2. 투척 속도 체크 (기준값 300.0f 등은 테스트하며 조절)
	float Speed = ThrowVelocity.Size();

	if (Speed > 500.f) // [의도: 투척]
	{
		// 기존 로직: 물리 켜고 날려보냄
		Collision->SetCollisionEnabled(ECollisionEnabled::QueryAndPhysics);
		Collision->SetCollisionProfileName(TEXT("PhysicsActor")); // Pawn은 Block 상태여야 함!

		// 안전장치
		//Collision->SetCollisionResponseToChannel(ECC_Pawn, ECR_Ignore);
		// 혹시 손이 Pawn 채널이 아니라 다른 채널(예: WorldDynamic)일 수도 있으니 안전장치
		// (필요하다면 추가) Collision->SetCollisionResponseToChannel(ECC_WorldDynamic, ECR_Ignore);

		if (GetInstigator())
		{
			Collision->IgnoreActorWhenMoving(GetInstigator(), true);
		}

		Collision->SetSimulatePhysics(true);
		Collision->SetPhysicsLinearVelocity(ThrowVelocity);
	}
	else
	{
		// 기본 동작: 손에서 떨어지고 물리 켜짐
		DetachFromActor(FDetachmentTransformRules::KeepWorldTransform);

		UPrimitiveComponent* RootPrim = Cast<UPrimitiveComponent>(GetRootComponent());
		if (RootPrim)
		{
			RootPrim->SetCollisionEnabled(ECollisionEnabled::QueryAndPhysics);
			if(!bIsGimmickProp) RootPrim->SetSimulatePhysics(true);
			RootPrim->SetPhysicsLinearVelocity(ThrowVelocity); // 기본 물리 적용
		}
		if(StaticMesh)
		{
			StaticMesh->SetCollisionEnabled(ECollisionEnabled::QueryAndPhysics);
			//StaticMesh->SetSimulatePhysics(true);
			//StaticMesh->SetPhysicsLinearVelocity(ThrowVelocity);
		}
	}
}

// Called every frame
void APropBase::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

