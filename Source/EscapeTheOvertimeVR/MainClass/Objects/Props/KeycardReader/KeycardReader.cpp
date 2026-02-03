#include "MainClass/Objects/Props/KeycardReader/KeycardReader.h"
#include "EscapeTheOvertimeCharacter.h"
#include "ElevatorDoor.h"
#include "Kismet/GameplayStatics.h"
#include "UObject/ConstructorHelpers.h"

AKeycardReader::AKeycardReader()
{
	// 사운드 큐 에셋 로드 (성공음)
	static ConstructorHelpers::FObjectFinder<USoundCue> SoundAsset(TEXT("/Engine/EditorSounds/Notifications/CompileFailed_Cue.CompileFailed_Cue"));

	if (SoundAsset.Succeeded())
	{
		AccessGrantedSound = SoundAsset.Object;
	}
}

void AKeycardReader::ActivateProp(AActor* Activator)
{
	AEscapeTheOvertimeCharacter* PlayerCharacter = Cast<AEscapeTheOvertimeCharacter>(Activator);

	if (!PlayerCharacter) return;

	// 카드키 소지 여부 확인 (Keycard.cpp에서 true로 설정된 값)
	if (PlayerCharacter->bHasKeycard)
	{
		UE_LOG(LogTemp, Warning, TEXT("Keycard Access Granted."));

		// 1. 카드키 소모 (사용 처리)
		// boolean 타입이므로 false로 되돌려 "감소/소멸" 효과를 냄
		PlayerCharacter->bHasKeycard = false;

		// 2. 인식 성공 사운드 재생
		if (AccessGrantedSound)
		{
			UGameplayStatics::PlaySoundAtLocation(this, AccessGrantedSound, GetActorLocation());
		}

		// 3. 엘리베이터 문 열기 로직 실행
		TArray<AActor*> EVDoors;
		UGameplayStatics::GetAllActorsOfClass(GetWorld(), AElevatorDoor::StaticClass(), EVDoors);

		for (AActor* EVDoor : EVDoors)
		{
			AElevatorDoor* OpenableDoor = Cast<AElevatorDoor>(EVDoor);
			if (OpenableDoor && !OpenableDoor->bIsOpen)
			{
				OpenableDoor->OpenDoor();
				UE_LOG(LogTemp, Warning, TEXT("Elevator Door opening..."));
			}
		}
	}
	else
	{
		UE_LOG(LogTemp, Warning, TEXT("Access Denied: Keycard required."));
	}
}

void AKeycardReader::Grab_Implementation(USceneComponent* HandController)
{
	ActivateProp(HandController->GetOwner());
}

void AKeycardReader::Release_Implementation(FVector ThrowVelocity)
{

}
