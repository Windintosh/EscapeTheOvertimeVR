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

void UETOGameInstance::SaveTime(float ElapsedTime, int32 Hour, float Minute)
{
	SavedElapsedTime = ElapsedTime;
	CurrentTimeHour = Hour;
	CurrentTimeMinute = Minute;
	UE_LOG(LogTemp, Warning, TEXT("Time Saved(ETOGI) %f / %d : %f"), SavedElapsedTime, CurrentTimeHour, CurrentTimeMinute);
}

float UETOGameInstance::LoadElapsedTime()
{
	return SavedElapsedTime;
}

int32 UETOGameInstance::LoadTimeHour()
{
	return CurrentTimeHour;
}

float UETOGameInstance::LoadTimeMinute()
{
	return CurrentTimeMinute;
}



void UETOGameInstance::SaveItemCounts(int32 DI, int32 TQA)
{
	DistractionItemCount = DI;
	TQAmmoCount = TQA;
	UE_LOG(LogTemp, Warning, TEXT("Items Saved - DI:%d, TQA:%d"), DistractionItemCount, TQAmmoCount);
}

int32 UETOGameInstance::LoadDistractionItemCount()
{
	return DistractionItemCount;
}

int32 UETOGameInstance::LoadTQAmmoCount()
{
	return TQAmmoCount;
}

void UETOGameInstance::SaveGotGolden(bool bGot)
{
	bPlayerGotGolden = bGot;
	//UE_LOG(LogTemp, Warning, TEXT("Saved Player got Golden: %s"), bPlayerGotGolden);
}

bool UETOGameInstance::LoadGotGolden()
{
	return bPlayerGotGolden;
}

