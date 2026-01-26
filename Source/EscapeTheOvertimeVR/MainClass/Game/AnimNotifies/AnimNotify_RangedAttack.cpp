#include "MainClass/Game/AnimNotifies/AnimNotify_RangedAttack.h"
#include "MainClass/Characters/Boss/EnemyProjectile.h"
#include "MainClass/Characters/Boss/MonkBoss.h"

void UAnimNotify_RangedAttack::Notify(USkeletalMeshComponent* MeshComp, UAnimSequenceBase* Animation)
{
	Super::Notify(MeshComp, Animation);

	AMonkBoss* MonkBoss = Cast<AMonkBoss>(MeshComp->GetOwner());
	if (!MonkBoss) return;

	if (MonkBoss && ProjectileClass)
	{
		FVector SpawnLocation = MeshComp->GetSocketLocation(TEXT("ProjectileSocket"));
		SpawnLocation += MonkBoss->GetActorForwardVector() * 50.0f; // move forward slightly
		FRotator SpawnRotation = MonkBoss->GetActorRotation();
		FTransform SpawnTransform(SpawnRotation, SpawnLocation, FVector(1.0f, 1.0f, 1.0f));

		FActorSpawnParameters SpawnParams;
		SpawnParams.Owner = MonkBoss;
		SpawnParams.Instigator = MonkBoss;

		SpawnParams.SpawnCollisionHandlingOverride = ESpawnActorCollisionHandlingMethod::AdjustIfPossibleButAlwaysSpawn; //ESpawnActorCollisionHandlingMethod::AlwaysSpawn;

		AEnemyProjectile* FlyingProjectile = GetWorld()->SpawnActor<AEnemyProjectile>(ProjectileClass, SpawnLocation, SpawnRotation, SpawnParams);
	}


}
