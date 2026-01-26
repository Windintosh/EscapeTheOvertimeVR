#include "MainClass/Game/AnimNotifies/AnimNotify_ThrowDO.h"
#include "AIController.h"
#include "BehaviorTree/BlackboardComponent.h"
#include "Kismet/KismetMathLibrary.h"
#include "Kismet/GameplayStatics.h"
#include "EscapeTheOvertimeCharacter.h"
#include "ThrownItem.h"


void UAnimNotify_ThrowDO::Notify(USkeletalMeshComponent* MeshComp, UAnimSequenceBase* Animation)
{
	Super::Notify(MeshComp, Animation);
	AEscapeTheOvertimeCharacter* PlayerCharacter = Cast<AEscapeTheOvertimeCharacter>(MeshComp->GetOwner());
	//MeshComp->GetSocketByName(TEXT("ItemSocket"));
	//if (!PlayerCharacter || !ThrownItemClass) return;

	if (PlayerCharacter && ThrownItemClass)
	{
		FVector SpawnLocation = MeshComp->GetSocketLocation(TEXT("ItemSocket"));
		SpawnLocation += PlayerCharacter->GetActorForwardVector() * 50.0f; // move forward slightly
		FRotator SpawnRotation = PlayerCharacter->GetActorRotation();

		FTransform SpawnTransform(SpawnRotation, SpawnLocation, FVector(1.0f, 1.0f, 1.0f)); //locks to scale 1

		//FActorSpawnParameters SpawnParams;
		//SpawnParams.Owner = PlayerCharacter;
		//SpawnParams.Instigator = PlayerCharacter;

		//SpawnParams.SpawnCollisionHandlingOverride = ESpawnActorCollisionHandlingMethod::AdjustIfPossibleButAlwaysSpawn; //ESpawnActorCollisionHandlingMethod::AlwaysSpawn;

		////spawn item to throw
		//AThrownItem* ThrownItem = GetWorld()->SpawnActor<AThrownItem>(ThrownItemClass, SpawnLocation, SpawnRotation, SpawnParams);
		//if (ThrownItem) 
		//{
		//	PlayerCharacter->DistractionItemQuantity--;
		//	UProjectileMovementComponent* ProjectileComp = ThrownItem->FindComponentByClass<UProjectileMovementComponent>();
		//	if (ProjectileComp)
		//	{
		//		// initial velocity set as player's direction
		//		FVector LaunchDirection = PlayerCharacter->GetActorForwardVector();
		//		ProjectileComp->Velocity = LaunchDirection * ProjectileComp->InitialSpeed;
		//	}
		//	ThrownItem->SetActorEnableCollision(false);
		//}

		// 2. [지연 소환 시작] 액터를 메모리에 할당만 하고, 아직 BeginPlay는 실행하지 않습니다.
		AThrownItem* ThrownItem = GetWorld()->SpawnActorDeferred<AThrownItem>(
			ThrownItemClass,
			SpawnTransform,
			PlayerCharacter, // Owner
			PlayerCharacter, // Instigator
			ESpawnActorCollisionHandlingMethod::AlwaysSpawn
		);


		if (ThrownItem)
		{
			// 3. [초기화] BeginPlay가 실행되기 '직전'에 필요한 설정을 여기서 합니다.

			// 예: 던지는 사람의 정보를 명확히 전달하거나 변수 설정 가능
			// ThrownItem->SetOwner(PlayerCharacter); (이미 위에서 했지만 재확인)

			// 4. [소환 완료] 이제 BeginPlay와 OnConstruction이 실행됩니다.
			// 물리 엔진이 이 시점에 올바른 크기와 Transform을 가지고 초기화됩니다.
			UGameplayStatics::FinishSpawningActor(ThrownItem, SpawnTransform);

			// 5. [발사] 소환이 완료된 후, Projectile Movement가 잘 작동하는지 확인
			UProjectileMovementComponent* PM = ThrownItem->FindComponentByClass<UProjectileMovementComponent>();
			if (PM)
			{
				// 만약 InitialSpeed로 자동 발사가 안 된다면 여기서 수동으로 속도 주입
				// PM->Velocity = PlayerCharacter->GetActorForwardVector() * PM->InitialSpeed;
			}

			PlayerCharacter->DistractionItemQuantity--;
		}

	}
}
