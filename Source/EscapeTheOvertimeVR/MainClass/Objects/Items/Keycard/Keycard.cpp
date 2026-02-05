#include "MainClass/Objects/Items/Keycard/Keycard.h"
#include "EscapeTheOvertimeCharacter.h" // 혹은 HorrorCharacter.h

AKeycard::AKeycard()
{
	ItemType = TEXT("Keycard");
	ItemIndex = 0;
}

void AKeycard::ActivateItem(AActor* Activator)
{
	AEscapeTheOvertimeCharacter* PlayerCharacter = Cast<AEscapeTheOvertimeCharacter>(Activator);

	// [자식만의 고유 기능] 카드키 획득 처리
	if (PlayerCharacter)
	{
		PlayerCharacter->bHasKeycard = true;
		UE_LOG(LogTemp, Warning, TEXT("Keycard Acquired!"));
	}

	//Super::ActivateItem(Activator);
}

void AKeycard::Grab_Implementation(USceneComponent* HandController)
{
	Super::Grab_Implementation(HandController);

	ActivateItem(HandController->GetOwner());
}

void AKeycard::Release_Implementation(FVector ThrowVelocity)
{
	AEscapeTheOvertimeCharacter* PlayerCharacter = GetWorld()->GetFirstPlayerController()->GetPawn<AEscapeTheOvertimeCharacter>();

	if (PlayerCharacter)
	{
		PlayerCharacter->bHasKeycard = false;
		UE_LOG(LogTemp, Warning, TEXT("Keycard Dropped!"));
	}

	if (bIsKeycardUsed) DestroyItem();

	Super::Release_Implementation(ThrowVelocity);

}


