#include "MainClass/Game/AnimNotifies/AnimNotify_PlayerNoise.h"
#include "HorrorCharacter.h"

FString UAnimNotify_PlayerNoise::GetNotifyName_Implementation() const
{
	return TEXT("Player makes Noise");
}

void UAnimNotify_PlayerNoise::Notify(USkeletalMeshComponent* MeshComp, UAnimSequenceBase* Animation)
{
	Super::Notify(MeshComp, Animation);

	AHorrorCharacter* Character = Cast<AHorrorCharacter>(MeshComp->GetOwner());
	if (!Character) return;

	if (!Character->bIsMuffled && Character->GetSprintState())
	{
		Character->MakeNoise(Loudness, Character->GetActorLocation());
	}
	
}
