#include "MainClass/Characters/Creatures/AnimNotify_MonkeyThrow.h"
#include "Monkey.h"

void UAnimNotify_MonkeyThrow::Notify(USkeletalMeshComponent* MeshComp, UAnimSequenceBase* Animation)
{
	Super::Notify(MeshComp, Animation);

	AMonkey* OwnerMonkey = Cast<AMonkey>(MeshComp->GetOwner());
	if (OwnerMonkey)
	{
		OwnerMonkey->ThrowBanana();
	}
	/*
	APawn* OwnerPawn = Cast<APawn>(MeshComp->GetOwner());	
	if (!OwnerPawn) return;	

	if (OwnerPawn && BananaClass)
	{
		FVector SpawnLocation = MeshComp->GetSocketLocation(TEXT("BananaSocket"));
		SpawnLocation += OwnerPawn->GetActorForwardVector() * 50.0f; // move forward slightly
		FRotator SpawnRotation = OwnerPawn->GetActorRotation();
		FTransform SpawnTransform(SpawnRotation, SpawnLocation, FVector(1.0f, 1.0f, 1.0f));
		FActorSpawnParameters SpawnParams;
		SpawnParams.Owner = OwnerPawn;
		SpawnParams.Instigator = OwnerPawn;
		SpawnParams.SpawnCollisionHandlingOverride = ESpawnActorCollisionHandlingMethod::AdjustIfPossibleButAlwaysSpawn; //ESpawnActorCollisionHandlingMethod::AlwaysSpawn;
		AMonkeyBanana* FlyingBanana = GetWorld()->SpawnActor<AMonkeyBanana>(BananaClass, SpawnLocation, SpawnRotation, SpawnParams);
	}*/
}
