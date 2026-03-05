#include "MainClass/Objects/Items/Essentials/Pill.h"
#include "HorrorCharacter.h"

APill::APill()
{
    ItemType = TEXT("Pill");
    ItemIndex = 998;
}

void APill::ActivateItem(AActor* Activator)
{
    AHorrorCharacter* PlayerCharacter = Cast<AHorrorCharacter>(Activator);
    if (!PlayerCharacter) return;

    PlayerCharacter->bGotPill = true;
    UE_LOG(LogTemp, Warning, TEXT("Pill Activated"));

    //부모 클래스 호출 (이 줄이 있어야 UI가 뜨고 아이템이 사라짐)
    Super::ActivateItem(Activator);
}
