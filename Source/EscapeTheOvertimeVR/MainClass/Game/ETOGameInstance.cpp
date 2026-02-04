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

