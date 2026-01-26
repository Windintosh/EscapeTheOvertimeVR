#include "MainClass/Objects/Items/Heal/MaxHPUpItem.h"
#include "HorrorCharacter.h"

AMaxHPUpItem::AMaxHPUpItem()
{
	ItemType = TEXT("MaxHPUpItem");
	ItemIndex = 5;
}

void AMaxHPUpItem::ActivateItem(AActor* Activator)
{
	AHorrorCharacter* PlayerCharacter = Cast<AHorrorCharacter>(Activator);
	if (!PlayerCharacter) return;

	PlayerCharacter->AddMaxHP(HealAmount);

	Super::ActivateItem(Activator);
}
