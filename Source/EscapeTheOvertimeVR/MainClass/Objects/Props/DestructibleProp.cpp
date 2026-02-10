#include "MainClass/Objects/Props/DestructibleProp.h"
#include "ThrownItem.h"

ADestructibleProp::ADestructibleProp()
{
}

void ADestructibleProp::BeginPlay()
{
	Super::BeginPlay();
}

void ADestructibleProp::OnPropOverlap(UPrimitiveComponent* OverlappedComp, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{
	if (bHasHealth) return;

	AThrownItem* ThrownItem = Cast<AThrownItem>(OtherActor);
	if (ThrownItem)
	{
		// ThrownItem과 충돌했을 때 데미지 처리
		HitCount--;
		UE_LOG(LogTemp, Warning, TEXT("DestructibleProp %s hit by ThrownItem %s. Remaining HitCount: %d"), *GetName(), *ThrownItem->GetName(), HitCount);
	}

	if(HitCount <= 0)
	{
		//Collision->SetSimulatePhysics(true);
		DestructProp();
		UE_LOG(LogTemp, Warning, TEXT("DestructibleProp %s has been destroyed due to HitCount reaching zero."), *GetName());
	}

	if (bIsDestroyed && OtherActor) DestructProp();
}

void ADestructibleProp::ActivateProp(AActor* Activator)
{
}

void ADestructibleProp::Tick(float DeltaTime)
{
}

float ADestructibleProp::TakeDamage(float DamageAmount, FDamageEvent const& DamageEvent, AController* EventInstigator, AActor* DamageCauser)
{
	if (!bHasHealth) return 0.f;

	const float ActualDamage = Super::TakeDamage(DamageAmount, DamageEvent, EventInstigator, DamageCauser);

	if (ActualDamage > 0)
	{
		Health -= ActualDamage;

		// 로그 출력 (디버깅용)
		UE_LOG(LogTemp, Warning, TEXT("Actor %s took %f damage from %s. Current Health: %f"), *GetName(), ActualDamage, *DamageCauser->GetName(), Health);

		if(Health <= 0.f)
		{
			bIsDestroyed = true;
			//Collision->SetSimulatePhysics(true);
			DestructProp();
			UE_LOG(LogTemp, Warning, TEXT("Actor %s has been destroyed."), *GetName());
		}
	}

	return ActualDamage;
}

void ADestructibleProp::DestructProp()
{
	//add effect or sth!
	DestroyProp();
}

