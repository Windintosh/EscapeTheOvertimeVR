#include "MainClass/Objects/Items/ItemBase.h"
#include "Variant_Horror/HorrorCharacter.h" // 경로: Variant_Horror
#include "Kismet/GameplayStatics.h"

// Sets default values
AItemBase::AItemBase()
{
	PrimaryActorTick.bCanEverTick = true;

	//Scene = CreateDefaultSubobject<USceneComponent>(TEXT("Scene"));
	//SetRootComponent(Scene);

	Collision = CreateDefaultSubobject<USphereComponent>(TEXT("Collision"));
	Collision->SetCollisionProfileName(TEXT("OverlapAllDynamic"));
	SetRootComponent(Collision);
	//Collision->SetupAttachment(Scene);

	StaticMesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("StaticMesh"));
	StaticMesh->SetupAttachment(Collision);

	Collision->InitSphereRadius(40.0f);
	Collision->OnComponentBeginOverlap.AddDynamic(this, &AItemBase::OnItemOverlap);
	Collision->OnComponentEndOverlap.AddDynamic(this, &AItemBase::OnItemEndOverlap);
}

// Called when the game starts or when spawned
void AItemBase::BeginPlay()
{
	Super::BeginPlay();
}

// Called every frame
void AItemBase::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
}

void AItemBase::OnItemOverlap(UPrimitiveComponent* OverlappedComp, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{
	// 1단계: 충돌 감지 디버깅
	if (GEngine)
		GEngine->AddOnScreenDebugMessage(-1, 5.f, FColor::Yellow, TEXT("[1] 충돌 감지됨!"));

	// 2단계: 플레이어 캐릭터인지 확인 (형변환)
	AHorrorCharacter* PlayerCharacter = Cast<AHorrorCharacter>(OtherActor);

	if (PlayerCharacter)
	{
		if (GEngine)
			GEngine->AddOnScreenDebugMessage(-1, 5.f, FColor::Green, TEXT("[2] 캐스팅 성공! 아이템 활성화..."));

		//ActivateItem(OtherActor);
	}
	else
	{
		// 실패 시 빨간 메시지
		FString FailMsg = FString::Printf(TEXT("[2] 캐스팅 실패! 부딪힌 액터는 AHorrorCharacter가 아닙니다. 이름: %s"), *OtherActor->GetName());

		if (GEngine)
			GEngine->AddOnScreenDebugMessage(-1, 5.f, FColor::Red, FailMsg);
	}
}

void AItemBase::OnItemEndOverlap(UPrimitiveComponent* OverlappedComp, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex)
{
	// 필요 시 구현
}

void AItemBase::ActivateItem(AActor* Activator)
{
	// 1. 플레이어를 HorrorCharacter로 캐스팅
	AHorrorCharacter* PlayerCharacter = Cast<AHorrorCharacter>(Activator);

	if (PlayerCharacter)
	{
		// 2. 델리게이트 방송 (Broadcast) -
		// 기존 ShowItemToast 함수 호출 대신 이벤트를 방송합니다.
		if (GEngine) GEngine->AddOnScreenDebugMessage(-1, 5.f, FColor::Cyan, TEXT("[3] 델리게이트 신호 발송!"));

		// 블루프린트에서 Assign(Bind)된 노드가 있는지 확인 후 방송
		// (IsBound 체크를 안 해도 Broadcast는 안전하지만, 디버깅을 위해 로그를 남깁니다)
		if (PlayerCharacter->OnItemPickedUp.IsBound())
		{
			PlayerCharacter->OnItemPickedUp.Broadcast(ItemDisplayName, ItemDescription, ItemIcon);
		}
		else
		{
			if (GEngine) GEngine->AddOnScreenDebugMessage(-1, 5.f, FColor::Orange, TEXT("[!] 경고: 블루프린트에서 OnItemPickedUp 노드가 연결되지 않았습니다."));
		}
	}

	// 3. 아이템 파괴 (공통 로직)
	DestroyItem();
}

FName AItemBase::GetItemType() const
{
	return ItemType;
}

int32 AItemBase::GetItemIndex() const
{
	return ItemIndex;
}

void AItemBase::DestroyItem()
{
	UE_LOG(LogTemp, Display, TEXT("%s is destroyed."), *GetName());
	Destroy();
}

void AItemBase::Grab_Implementation(USceneComponent* HandController)
{
	if (!bIsGrabbable) return;

	ActivateItem(HandController->GetOwner());

	// 기본 동작: 물리 끄고 손에 붙기
	UPrimitiveComponent* RootPrim = Cast<UPrimitiveComponent>(GetRootComponent());
	if (RootPrim)
	{
		RootPrim->SetSimulatePhysics(false);
	}

	AttachToComponent(HandController, FAttachmentTransformRules::SnapToTargetNotIncludingScale, NAME_None);
}

void AItemBase::Release_Implementation(FVector ThrowVelocity)
{
	// 기본 동작: 손에서 떨어지고 물리 켜짐
	DetachFromActor(FDetachmentTransformRules::KeepWorldTransform);

	UPrimitiveComponent* RootPrim = Cast<UPrimitiveComponent>(GetRootComponent());
	if (RootPrim)
	{
		RootPrim->SetCollisionEnabled(ECollisionEnabled::QueryAndPhysics);
		if(!bIsGimmickItem) RootPrim->SetSimulatePhysics(true);
		RootPrim->SetPhysicsLinearVelocity(ThrowVelocity); // 기본 물리 적용
	}
}

void AItemBase::OnAction_Implementation(float Value)
{

}

//flag for the file change