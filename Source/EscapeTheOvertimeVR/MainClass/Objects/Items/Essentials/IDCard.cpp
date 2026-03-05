#include "MainClass/Objects/Items/Essentials/IDCard.h"
#include "HorrorCharacter.h"


AIDCard::AIDCard()
{
	ItemType = TEXT("IDCard");
    ItemIndex = 997;
}

void AIDCard::ActivateItem(AActor* Activator)
{
    AHorrorCharacter* PlayerCharacter = Cast<AHorrorCharacter>(Activator);
    if (PlayerCharacter)
    {
        PlayerCharacter->bGotIDCard = true;
    }
	Super::ActivateItem(Activator);
}
