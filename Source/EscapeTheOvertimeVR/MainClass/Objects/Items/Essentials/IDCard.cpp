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
        UE_LOG(LogTemp, Warning, TEXT("Player Got ID Card"));
        GEngine->AddOnScreenDebugMessage(-1, 5.f, FColor::Green, TEXT("Player Got ID Card"));
    }
	Super::ActivateItem(Activator);
}
