#include "MainClass/Objects/Items/Muffle/MuffleItem.h"
#include "EscapeTheOvertimeCharacter.h"

AMuffleItem::AMuffleItem()
{
	ItemType = TEXT("MuffleItem");
	ItemIndex = 1;
}

void AMuffleItem::ActivateItem(AActor* Activator)
{
	AEscapeTheOvertimeCharacter* PlayerCharacter = Cast<AEscapeTheOvertimeCharacter>(Activator);
	if (!PlayerCharacter) return;

	//PlayerCharacter->bIsMuffled = true;
	//PlayerCharacter->MuffleItem.RemainingTime = PlayerCharacter->MuffleItem.Duration;
	PlayerCharacter->ActivateMuffle();

	//부모 클래스 호출 (이 줄이 있어야 UI가 뜨고 아이템이 사라짐)
	Super::ActivateItem(Activator);
}
