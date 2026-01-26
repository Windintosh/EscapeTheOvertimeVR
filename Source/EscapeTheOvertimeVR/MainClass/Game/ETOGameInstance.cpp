#include "MainClass/Game/ETOGameInstance.h"

UETOGameInstance::UETOGameInstance()
{

}

void UETOGameInstance::SavePlayerHP(float HP)
{
	PlayerMaxHP = HP;
	UE_LOG(LogTemp, Warning, TEXT("Health Saved(ETOGI) %f"), PlayerMaxHP);
}

float UETOGameInstance::LoadPlayerHP()
{
	return PlayerMaxHP;
}
