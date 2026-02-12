#include "MainClass/Objects/Items/Distraction/ThrownItem.h"
#include "Boss/Boss.h"
#include "HorrorCharacter.h"
#include "VRCharacterPawn.h"
#include "Kismet/GameplayStatics.h"

AThrownItem::AThrownItem()
{
	ItemType = TEXT("ThrownItem");
	ItemIndex = 999;

	// 부모에서 설정했어도 안전을 위해 확인
	if (Collision) SetRootComponent(Collision);

	// Static Mesh 충돌 끄기 (확실하게)
	if (StaticMesh)
	{
		StaticMesh->SetCollisionEnabled(ECollisionEnabled::NoCollision);
		StaticMesh->SetCollisionProfileName(TEXT("NoCollision"));
	}

	// [핵심] Collision 설정 강화
	if (Collision)
	{
		Collision->SetCollisionProfileName(TEXT("PhysicsActor")); // [변경] VR 물체는 PhysicsActor가 적절
		Collision->SetCollisionEnabled(ECollisionEnabled::QueryAndPhysics);
		Collision->SetNotifyRigidBodyCollision(true);
		Collision->SetSimulatePhysics(true); // [핵심] 기본적으로 물리 적용 (바닥에 놓여있도록)

	}

	GeometryCollectionComponent = CreateDefaultSubobject<UGeometryCollectionComponent>(TEXT("GeometryCollectionComponent"));
	GeometryCollectionComponent->SetupAttachment(RootComponent);

	if (RestCollectionAsset)
	{
		GeometryCollectionComponent->SetNotifyBreaks(true);
		GeometryCollectionComponent->SetCollisionEnabled(ECollisionEnabled::NoCollision);
		GeometryCollectionComponent->SetSimulatePhysics(false);
	}

}

void AThrownItem::OnItemOverlap(UPrimitiveComponent* OverlappedComp, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{
	//ACharacter* PlayerCharacter = UGameplayStatics::GetPlayerCharacter(GetWorld(), 0);
	//AHorrorCharacter* GameCharacter = Cast<AHorrorCharacter>(PlayerCharacter);

	////GeometryCollectionComponent->SetSimulatePhysics(true);
	//GameCharacter->MakeNoise(5.f, GetActorLocation()); //Make noise at overlapped point, add condition(if boss) if glitches

	//ActivateItem(OtherActor); //do sth to boss
}

void AThrownItem::OnItemHit(UPrimitiveComponent* HitComp, AActor* OtherActor, UPrimitiveComponent* OtherComp, FVector NormalImpulse, const FHitResult& Hit)
{

	if (bIsBroken || (OtherActor == nullptr) || (OtherActor == this) || (OtherActor == GetInstigator()) || (OtherActor == GetWorld()->GetFirstPlayerController()->GetPawn()))
	{
		return;
	}

	// 2. [핵심] 던져진 상태가 아니라면 절대 깨지지 않음! (그냥 굴러다니게 둠)
	if (!bWasThrown)
	{
		UE_LOG(LogTemp, Log, TEXT("Hit Ignore: Item was NOT thrown."));
		return;
	}

	// [조건 추가] 충격량이 너무 작으면 깨지지 않게 함 (살짝 닿았을 때 방지)
	// ProjectileMovement를 안 쓰므로 Velocity가 아니라 충격량(Impulse)으로 판단 추천
	float ImpactForce = NormalImpulse.Size();
	UE_LOG(LogTemp, Error, TEXT("OnHit Called! Impact: %f, Hit Actor: %s"), ImpactForce, *OtherActor->GetName());
	if (ImpactForce < 10.0f) return; // 임계값 조절 필요

	bIsBroken = true;

	if (CrashSound)
	{
		UGameplayStatics::PlaySoundAtLocation(this, CrashSound, GetActorLocation());
	}


	//ACharacter* PlayerCharacter = UGameplayStatics::GetPlayerCharacter(GetWorld(), 0);
	//AHorrorCharacter* GameCharacter = Cast<AHorrorCharacter>(PlayerCharacter);
	////ABoss* BossCharacter = Cast<ABoss>(PlayerCharacter);

	////GeometryCollectionComponent->SetSimulatePhysics(true);
	//GameCharacter->MakeNoise(5.f, GetActorLocation()); //Make noise at overlapped point, add condition(if boss) if glitches

	// [수정 후] 안전한 범용 코드 (VR Pawn, FPS 캐릭터 모두 호환)
	APawn* PlayerPawn = UGameplayStatics::GetPlayerPawn(GetWorld(), 0);
	if (PlayerPawn)
	{
		// 엔진 기본 MakeNoise 사용 (안전함)
		// Loudness: 1.0f (0~1 사이 권장), Range: 적절히 조절
		PlayerPawn->MakeNoise(1.0f, PlayerPawn, GetActorLocation());

		UE_LOG(LogTemp, Log, TEXT("MakeNoise Triggered by PlayerPawn"));
	}

	if (OtherActor && (OtherActor != PlayerPawn || OtherActor != GetInstigator()))
	{
		ActivateItem(OtherActor); //do sth to boss

		Damage = 20.f + ImpactForce * 0.01;

		OtherActor->TakeDamage(Damage, DamageEvent, GetInstigatorController(), this);
		UE_LOG(LogTemp, Log, TEXT("%f Damage Applied to %s"), Damage, *OtherActor->GetName());
	}

	// 1. 둥둥 떠있게 될 Static Mesh를 숨깁니다.
	if (StaticMesh)
	{
		StaticMesh->SetVisibility(false);
	}

	// 2. 루트(Sphere)의 충돌을 끕니다. (더 이상 충돌 방지)
	if (Collision)
	{
		//Collision->SetSimulatePhysics(false); // 더 이상 굴러다니지 않게 멈춤
		Collision->SetCollisionEnabled(ECollisionEnabled::NoCollision);
		Collision->SetCollisionResponseToAllChannels(ECR_Ignore);
	}

	//AActor* Breaker;
	if (GeometryCollectionComponent)
	{
		GeometryCollectionComponent->SetVisibility(true);
		GeometryCollectionComponent->SetCollisionProfileName(TEXT("PhysicsActor")); // 또는 BlockAllDynamic
		GeometryCollectionComponent->SetCollisionEnabled(ECollisionEnabled::QueryAndPhysics);
		GeometryCollectionComponent->SetNotifyBreaks(true);
		GeometryCollectionComponent->SetSimulatePhysics(true);

		// 물리 상태 강제 리셋 (잠든 상태 깨우기)
		GeometryCollectionComponent->RecreatePhysicsState();

		// [안전장치 1] 속도가 0이어도 방향을 구함 (정면 벡터 사용)
		FVector ImpulseDir = -Hit.ImpactNormal;
		if (ImpulseDir.IsZero()) ImpulseDir = GetActorForwardVector();

		// [안전장치 2] 파괴 원점(Origin) 계산
		FVector BreakOrigin = Hit.ImpactPoint;
		if (BreakOrigin.IsZero()) BreakOrigin = GetActorLocation();

		// A. 내부 파괴 (Strain) - 확실하게 깨뜨림
		GeometryCollectionComponent->ApplyExternalStrain(
			1000000.0f, // 백만 (확실한 파괴)
			BreakOrigin,
			50.0f
		);

		FVector CenterLocation = GetActorLocation();

		// Strength: 2000.0f 정도 (너무 세면 우주로 날아가니 조절 필요)
		// Radius: 50.0f (머그컵 크기 정도)
		// bVelChange: true (질량 무시하고 즉각 속도 변경)
		GeometryCollectionComponent->AddRadialImpulse(CenterLocation, 50.0f, 2000.0f, ERadialImpulseFalloff::RIF_Linear, true);

		// B. 외부 충격 (Impulse) - 파편 흩뿌리기
		// bVelChange=false로 설정하고 질량 고려한 힘을 가함 (너무 빠르지 않게)
		GeometryCollectionComponent->AddImpulse(ImpulseDir * 500.f, NAME_None, false);

		// [필살기] 외부 파괴자(Field System) 소환

		if (BreakerClass)
		{
			UE_LOG(LogTemp, Warning, TEXT("Attempting to Spawn Breaker...")); // 로그 1
			FVector SpawnLoc = GetActorLocation();
			FRotator SpawnRot = FRotator::ZeroRotator;

			// 소환되는 순간 주변 GC를 박살냄
			AActor* Breaker = GetWorld()->SpawnActor<AActor>(BreakerClass, SpawnLoc, SpawnRot);
			if (Breaker)
			{
				UE_LOG(LogTemp, Warning, TEXT("Breaker Spawned SUCCESSFULLY: %s"), *Breaker->GetName()); // 로그 2
				Breaker->Destroy();
				UE_LOG(LogTemp, Display, TEXT("Breaker %s Destroyed"), *Breaker->GetName());
			}
			else
			{
				UE_LOG(LogTemp, Error, TEXT("Breaker Spawn FAILED!")); // 로그 3
			}
		}
		else
		{
			UE_LOG(LogTemp, Error, TEXT("BreakerClass is NULL! Please assign in BP!")); // 로그 4 (할당 안 됨)
		}
	}

	// 6. 0.2초 뒤 플레이어 충돌 무시 (이전에 만든 함수)
	GetWorld()->GetTimerManager().SetTimer(CollisionTimerHandle, this, &AThrownItem::DisablePawnCollision, 0.2f, false);


	MakeNoise(3.0f, GetWorld()->GetFirstPlayerController()->GetCharacter(), GetActorLocation());


	SetLifeSpan(5.0f); //
}

void AThrownItem::ActivateItem(AActor* Activator)
{
	ABoss* Boss = Cast<ABoss>(Activator);
	if (Boss) IBossHit::Execute_Interact(Boss, this);	//stun Boss ,Make boss stunned or sth
}

void AThrownItem::BeginPlay()
{
	Super::BeginPlay();

	//DrawDebugSphere(GetWorld(), GetActorLocation(), Collision->GetScaledSphereRadius(), 12, FColor::Red, true, 5.0f);

	//Collision->OnComponentBeginOverlap.RemoveDynamic(this, &AItemBase::OnItemOverlap);
	if (Collision)
	{
		// 3. ignore instigator(who threw)
		if (GetInstigator())
		{
			Collision->MoveIgnoreActors.Add(GetInstigator()); // ignore collision when moving
			// or
			Collision->IgnoreActorWhenMoving(GetInstigator(), true);
		}
		Collision->OnComponentHit.RemoveDynamic(this, &AThrownItem::OnItemHit);
		Collision->OnComponentHit.AddDynamic(this, &AThrownItem::OnItemHit);

		//Collision->SetSimulatePhysics(true);
		Collision->SetCollisionEnabled(ECollisionEnabled::QueryAndPhysics);
		Collision->SetNotifyRigidBodyCollision(true); // Hit Event 발생 필수
		//Collision->SetSimulatePhysics(true); // 물리 엔진 가동

		Collision->SetUseCCD(true);
	}

	// 디버깅: 실제 반경이 몇인지 로그로 확인
	if (Collision)
	{
		float Radius = Collision->GetScaledSphereRadius();
		UE_LOG(LogTemp, Warning, TEXT("Current Sphere Radius: %f"), Radius);

		if (Radius <= 0.1f)
		{
			UE_LOG(LogTemp, Error, TEXT("CRITICAL ERROR: Radius is Zero! Force setting it."));
			Collision->SetSphereRadius(7.f);
			Collision->SetCollisionEnabled(ECollisionEnabled::QueryAndPhysics);
		}
	}

	if (StaticMesh)
	{
		StaticMesh->SetVisibility(true);
		StaticMesh->SetCollisionEnabled(ECollisionEnabled::NoCollision); // 충돌은 오직 Sphere가 담당
	}

	// 3. GC 물리 끄기 (날아가는 동안 방해 안 되게)
	if (GeometryCollectionComponent)
	{
		GeometryCollectionComponent->SetVisibility(false);
		GeometryCollectionComponent->SetSimulatePhysics(false);
		GeometryCollectionComponent->SetCollisionEnabled(ECollisionEnabled::NoCollision);
		GeometryCollectionComponent->SetNotifyBreaks(true);
	}

	AVRCharacterPawn* VRPawn = Cast<AVRCharacterPawn>(UGameplayStatics::GetPlayerPawn(GetWorld(), 0));
	if (VRPawn->bGotGolden || bIsGolden)
	{
		ChangeToGoldenItem();
	}
}

void AThrownItem::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	//UE_LOG(LogTemp, Warning, TEXT("TI Location X: %f, Y: %f, Z: %f"), GetActorLocation().X, GetActorLocation().Y, GetActorLocation().Z);
}

void AThrownItem::DisablePawnCollision()
{
	if (GeometryCollectionComponent)
	{

		// 1. [핵심] 물체의 '신분(Object Type)'을 방금 만든 Debris 채널로 변경합니다.
		// 'ECC_GameTraceChannel1'은 보통 첫 번째로 만든 커스텀 채널(Debris)을 의미합니다.
		// (Project Settings -> Collision -> Object Channels 목록 순서 확인 필요하지만 보통 1번입니다.)
		GeometryCollectionComponent->SetCollisionObjectType(ECC_GameTraceChannel2);

		// 2. 충돌 반응 재설정 (솔루션 B 방식 유지)
		// 일단 다 무시하고...
		GeometryCollectionComponent->SetCollisionResponseToAllChannels(ECR_Ignore);

		// 바닥(WorldStatic)과 벽(WorldDynamic)은 밟고 서 있어야 하니 Block으로 켭니다.
		GeometryCollectionComponent->SetCollisionResponseToChannel(ECC_WorldStatic, ECR_Block);
		GeometryCollectionComponent->SetCollisionResponseToChannel(ECC_WorldDynamic, ECR_Block);

		// 플레이어(Pawn)는 Debris 채널을 'Ignore'하도록 설정했으므로, 
		// 여기서 굳이 ECR_Ignore를 안 해도 무시되지만, 확실하게 하기 위해 둡니다.
		GeometryCollectionComponent->SetCollisionResponseToChannel(ECC_Pawn, ECR_Ignore);

		// 카메라 무시 (시야 가림 방지)
		GeometryCollectionComponent->SetCollisionResponseToChannel(ECC_Camera, ECR_Ignore);

		UE_LOG(LogTemp, Warning, TEXT("Pawn Collision Disabled for Debris!"));
	}

	// 혹시 모르니 루트 컴포넌트(구체)도 다시 한 번 확실하게 끕니다.
	if (Collision)
	{
		Collision->SetCollisionEnabled(ECollisionEnabled::NoCollision);
	}
}

void AThrownItem::ReEnablePawnCollision()
{
	if (Collision && !bIsBroken)
	{
		//Collision->SetCollisionResponseToChannel(ECC_Pawn, ECR_Block);
		// 특정 액터 무시 해제
		Collision->IgnoreActorWhenMoving(GetInstigator(), false);
	}
}

void AThrownItem::OnConstruction(const FTransform& Transform)
{
	Super::OnConstruction(Transform);

	if (Collision)
	{
		// 1. 크기가 0이 되지 않도록 강제 설정
		if (Collision->GetUnscaledSphereRadius() <= 0.0f)
		{
			Collision->SetSphereRadius(7.f); // 원하는 크기로 설정
		}

		// 2. 물리 상태 강제 재생성 (구체가 안 보일 때 특효약)
		// 이 함수는 "물리 엔진아, 나 여기 있어! 다시 그려줘"라고 명령하는 것입니다.
		Collision->RecreatePhysicsState();
	}

}

void AThrownItem::Grab_Implementation(USceneComponent* HandController)
{
	if (bIsBroken || !Collision) return;

	if (bIsGolden) GrantGold();

	bWasThrown = false;

	// 1. 물리 끄기 (손에 붙이기 위해)
	Collision->SetSimulatePhysics(false);

	// 3. 충돌 처리: 잡고 있는 동안은 Pawn과 부딪히지 않게 함 (떨림 방지)
	Collision->SetCollisionResponseToChannel(ECC_Pawn, ECR_Ignore);

	// 4. 손에 부착 (SnapToTarget을 쓰면 손 위치로 강제 이동, KeepWorld는 잡은 위치 유지)
	// 여기서는 자연스러운 잡기를 위해 KeepWorld를 추천하지만, 
	// 총처럼 딱 잡히길 원하면 SnapToTarget 사용
	FAttachmentTransformRules AttachmentRules(EAttachmentRule::KeepWorld, EAttachmentRule::KeepWorld, EAttachmentRule::KeepWorld, true);
	AttachToComponent(HandController, AttachmentRules);

	UE_LOG(LogTemp, Log, TEXT("ThrownItem Grabbed"));
}

void AThrownItem::Release_Implementation(FVector ThrowVelocity)
{
	if (bIsBroken || !Collision) return;

	// 1. 손에서 분리
	DetachFromActor(FDetachmentTransformRules::KeepWorldTransform);

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
		Collision->SetPhysicsLinearVelocity(ThrowVelocity * ThrowPowerMultiplier);

		// (선택) 회전력(Torque)을 약간 주면 더 리얼하게 날아감 (랜덤 회전)
		//Collision->SetPhysicsAngularVelocityInDegrees(FVector(FMath::RandRange(-360, 360), FMath::RandRange(-360, 360), 0));

		bWasThrown = true;
		//GetWorld()->GetTimerManager().SetTimer(IgnorePawnTimerHandle, this, &AThrownItem::ReEnablePawnCollision, 0.5f, false);

		AVRCharacterPawn* VRPawn = Cast<AVRCharacterPawn>(UGameplayStatics::GetPlayerCharacter(GetWorld(), 0));
		if (VRPawn)
		{
			if((bIsSpawned || bIsSpawnedItem) && !VRPawn->bGotGolden)
				VRPawn->DistractionItemQuantity--;
			UE_LOG(LogTemp, Log, TEXT("Mug Thrown; Player has left: %d"), VRPawn->DistractionItemQuantity);
		}

		UE_LOG(LogTemp, Log, TEXT("Mug Thrown! (Speed: %f)"), Speed);
	}
	else // [의도: 소환 취소 / 살짝 놓기]
	{
		if (!bIsSpawned || !bIsSpawnedItem)
		{
			AVRCharacterPawn* VRPawn = Cast<AVRCharacterPawn>(UGameplayStatics::GetPlayerCharacter(GetWorld(), 0));
			if (VRPawn)
			{
				VRPawn->DistractionItemQuantity++;
				UE_LOG(LogTemp, Log, TEXT("Mug is not spawned; Player took this one: %d"), VRPawn->DistractionItemQuantity);
			}
		}
		// 물리 켜지 말고 바로 제거
		UE_LOG(LogTemp, Log, TEXT("Mug Dropped gently -> Destroyed (Unequipped)"));
		Destroy();
	}


}

void AThrownItem::ChangeThrownState()
{
	bWasThrown = !bWasThrown;
}

void AThrownItem::ChangeToGoldenItem()
{
	bIsGolden = true;
	StaticMesh->SetMaterial(0, GoldenMaterial);
	GeometryCollectionComponent->SetMaterial(0, GoldenMaterial);
}

void AThrownItem::GrantGold()
{
	if (!bIsGolden) return;
	AVRCharacterPawn* VRPawn = Cast<AVRCharacterPawn>(UGameplayStatics::GetPlayerCharacter(GetWorld(), 0));
	if (VRPawn)
	{
		VRPawn->bGotGolden = true;
		UE_LOG(LogTemp, Log, TEXT("Player got the Golden Mug!"));
	}
}
