#include "MainClass/Objects/Props/GrabbableProp.h"
#include "Components/StaticMeshComponent.h"
#include "Components/BoxComponent.h"

AGrabbableProp::AGrabbableProp()
{
	PrimaryActorTick.bCanEverTick = false;

	// 1. 박스 콜리전 생성 및 루트 설정 (물리 본체)
	GrabCollision = CreateDefaultSubobject<UBoxComponent>(TEXT("GrabCollision"));
	SetRootComponent(GrabCollision);

	// 박스 크기 기본값 (블루프린트에서 조정 가능)
	GrabCollision->SetBoxExtent(FVector(10.f, 10.f, 2.f));

	// 2. 메쉬 컴포넌트 생성 및 박스에 부착
	PropMesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("PropMesh"));
	PropMesh->SetupAttachment(GrabCollision);

	// 메쉬는 물리를 끄고 충돌도 끕니다 (박스가 다 하므로)
	PropMesh->SetSimulatePhysics(false);
	PropMesh->SetCollisionProfileName(TEXT("NoCollision"));

	// 3. 박스 물리 설정
	GrabCollision->SetSimulatePhysics(true);
	GrabCollision->SetCollisionProfileName(TEXT("PhysicsActor")); // 혹은 Custom
	GrabCollision->SetGenerateOverlapEvents(true); // 손 감지용

	// 물리 안정화 설정
	GrabCollision->SetMassOverrideInKg(NAME_None, 1.0f, true); // 1kg 고정
	GrabCollision->SetLinearDamping(1.0f); // 공기 저항
	GrabCollision->SetAngularDamping(1.0f); // 회전 저항

	bIsHeld = false;
	HoldingHand = nullptr;
}

void AGrabbableProp::BeginPlay()
{
	Super::BeginPlay();
}

void AGrabbableProp::Grab(USceneComponent* Hand)
{
	if (!Hand || bIsHeld) return;

	// 루트(박스)의 물리를 끕니다.
	GrabCollision->SetSimulatePhysics(false);

	// 손에 부착 (위치 유지)
	FAttachmentTransformRules AttachmentRules(EAttachmentRule::KeepWorld, true);
	AttachToComponent(Hand, AttachmentRules);

	HoldingHand = Hand;
	bIsHeld = true;
}

void AGrabbableProp::Release()
{
	if (!bIsHeld) return;

	// 손에서 분리
	FDetachmentTransformRules DetachmentRules(EDetachmentRule::KeepWorld, true);
	DetachFromActor(DetachmentRules);

	// 물리 다시 켜기
	GrabCollision->SetSimulatePhysics(true);

	HoldingHand = nullptr;
	bIsHeld = false;
}