#include "MainClass/Objects/Items/Distraction/DistractionItem.h"
#include "EscapeTheOvertimeCharacter.h"

ADistractionItem::ADistractionItem()
{
	ItemType = TEXT("DistractionItem");
	ItemIndex = 3;
}

void ADistractionItem::ActivateItem(AActor* Activator)
{
	AEscapeTheOvertimeCharacter* PlayerCharacter = Cast<AEscapeTheOvertimeCharacter>(Activator);
	if (!PlayerCharacter) return;

	//distract AI(x) or obtain it for use()
	PlayerCharacter->ActivateDistraction();


	//부모 클래스 호출 (이 줄이 있어야 UI가 뜨고 아이템이 사라짐)
	Super::ActivateItem(Activator);
}
