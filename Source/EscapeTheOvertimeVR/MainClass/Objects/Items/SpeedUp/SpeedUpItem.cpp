#include "MainClass/Objects/Items/SpeedUp/SpeedUpItem.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "EscapeTheOvertimeCharacter.h"
#include "HorrorCharacter.h"

ASpeedUpItem::ASpeedUpItem()
{
	ItemType = TEXT("SpeedUpItem");
	ItemIndex = 2;
}

void ASpeedUpItem::ActivateItem(AActor* Activator)
{
	AHorrorCharacter* PlayerCharacter = Cast<AHorrorCharacter>(Activator);
	if (!PlayerCharacter) return;

	//PlayerCharacter->bIsSpedUp = true;
	//PlayerCharacter->GetCharacterMovement()->MaxWalkSpeed = PlayerCharacter->GetCharacterMovement()->MaxWalkSpeed * 2;
	//PlayerCharacter->SpeedUpItem.RemainingTime = PlayerCharacter->SpeedUpItem.Duration;
	PlayerCharacter->ActivateSpeedUp();

	//부모 클래스 호출 (이 줄이 있어야 UI가 뜨고 아이템이 사라짐)
	Super::ActivateItem(Activator);
}
