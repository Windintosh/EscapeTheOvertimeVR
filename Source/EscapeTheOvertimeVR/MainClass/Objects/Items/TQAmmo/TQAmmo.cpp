#include "MainClass/Objects/Items/TQAmmo/TQAmmo.h"
#include "EscapeTheOvertimeCharacter.h"

ATQAmmo::ATQAmmo()
{
	ItemType = TEXT("TQAmmo");
	ItemIndex = 6;
}

void ATQAmmo::ActivateItem(AActor* Activator)
{
	AEscapeTheOvertimeCharacter* PlayerCharacter = Cast<AEscapeTheOvertimeCharacter>(Activator);
	if (!PlayerCharacter) return;
	
	PlayerCharacter->ActivateTQAmmo();

	// Call parent class implementation to handle UI display and item destruction
	Super::ActivateItem(Activator);


}
