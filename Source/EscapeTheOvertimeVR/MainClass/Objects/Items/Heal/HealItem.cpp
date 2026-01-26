#include "MainClass/Objects/Items/Heal/HealItem.h"
#include "HorrorCharacter.h"

AHealItem::AHealItem()
{
	ItemType = TEXT("HealItem");
	ItemIndex = 4;
}

void AHealItem::ActivateItem(AActor* Activator)
{
	AHorrorCharacter* PlayerCharacter = Cast<AHorrorCharacter>(Activator);
	if (!PlayerCharacter) return;

	PlayerCharacter->Heal(HealAmount);

	//부모 클래스 호출 (이 줄이 있어야 UI가 뜨고 아이템이 사라짐)
	Super::ActivateItem(Activator);
}
