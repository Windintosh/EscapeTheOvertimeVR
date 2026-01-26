#include "MainClass/Characters/Boss/EnemyProjectile.h"
#include "Variant_Horror/HorrorCharacter.h"
#include "Kismet/GameplayStatics.h"

// Sets default values
AEnemyProjectile::AEnemyProjectile()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	//Scene = CreateDefaultSubobject<USceneComponent>(TEXT("Scene"));
	//SetRootComponent(Scene);

	Collision = CreateDefaultSubobject<USphereComponent>(TEXT("Collision"));
	Collision->SetCollisionProfileName(TEXT("OverlapAllDynamic"));
	Collision->SetCollisionEnabled(ECollisionEnabled::QueryOnly);
	Collision->SetCollisionObjectType(ECC_WorldDynamic);
	Collision->SetCollisionResponseToAllChannels(ECR_Overlap);
	Collision->SetCollisionResponseToChannel(ECC_Pawn, ECR_Overlap);
	Collision->SetCollisionResponseToChannel(ECC_WorldStatic, ECR_Overlap);
	Collision->SetCollisionResponseToChannel(ECC_WorldDynamic, ECR_Overlap);
	Collision->SetGenerateOverlapEvents(true);
	Collision->InitSphereRadius(40.0f);
	SetRootComponent(Collision);
	//Collision->SetupAttachment(Scene);

	StaticMesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("StaticMesh"));
	StaticMesh->SetupAttachment(Collision);
	StaticMesh->SetCollisionProfileName(TEXT("NoCollision"));
	
	
}

// Called when the game starts or when spawned
void AEnemyProjectile::BeginPlay()
{
	Super::BeginPlay();

	Collision->OnComponentBeginOverlap.AddDynamic(this, &AEnemyProjectile::OnPlayerOverlap);
	
}

void AEnemyProjectile::OnPlayerOverlap(UPrimitiveComponent* OverlappedComp, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{
	UE_LOG(LogTemp, Error, TEXT("Projectile: Overlapped Object %s"), *OtherActor->GetName());
	if (!OtherActor || OtherActor == this || OtherActor == GetOwner())
	{
		UE_LOG(LogTemp, Error, TEXT("Projectile: Overlapped Object is not a Player!"));
		return;
	}
	
	AHorrorCharacter* PlayerCharacter = Cast<AHorrorCharacter>(OtherActor);
	if (PlayerCharacter)
	{
		// 플레이어라면 대미지를 주고
		//PlayerCharacter->DamagePlayer(40.f);
		AController* OwnerController = nullptr;
		if (GetOwner())
		{
			OwnerController = GetOwner()->GetInstigatorController();
		}

		// 3. 엔진 표준 대미지 전달 함수 호출
		UGameplayStatics::ApplyDamage(
			OtherActor,                     // 맞는 사람
			20.0f,                          // 대미지 양
			OwnerController,                // 때린 사람의 컨트롤러 (Instigator)
			this,                           // 대미지를 가한 도구 (Projectile)
			UDamageType::StaticClass()      // 대미지 타입 (기본형)
		);
		
		// 발사체 파괴
		DestroyProjectile();
	}
	else
	{
		// 3. 플레이어가 아니지만 충돌한 경우 (벽, 바닥 등)
		DestroyProjectile();
	}
}

void AEnemyProjectile::DestroyProjectile()
{
	UE_LOG(LogTemp, Display, TEXT("%s is destroyed."), *GetName());
	Destroy();
}

// Called every frame
void AEnemyProjectile::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	FVector NewLocation = GetActorLocation() + GetActorForwardVector() * Speed * DeltaTime;

	FHitResult Hit;

	SetActorLocation(NewLocation, true, &Hit); //bSweep = true to check collision

	Collision->UpdateOverlaps();

	if (Hit.IsValidBlockingHit())
	{
		DestroyProjectile();
	}	

	if (Collision)
	{
		DrawDebugSphere(GetWorld(), Collision->GetComponentLocation(), Collision->GetScaledSphereRadius(), 12, FColor::Red, false, -1.0f);
	}
}

