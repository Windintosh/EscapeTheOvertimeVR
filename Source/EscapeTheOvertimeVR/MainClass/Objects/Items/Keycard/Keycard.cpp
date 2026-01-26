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

	Super::ActivateItem(Activator);
}
