#include "MainClass/Objects/Doors/MainDoors/GimmickDoor.h" 
#include "Kismet/GameplayStatics.h"
#include "Sound/SoundBase.h"

AGimmickDoor::AGimmickDoor()
{
	// 기본적으로 잠겨있는 상태로 시작
	bIsLocked = true;
}

void AGimmickDoor::BeginPlay()
{
	Super::BeginPlay();
}

void AGimmickDoor::Grab_Implementation(USceneComponent* HandController)
{
	// 1. 잠겨있는지 확인
	if (bIsLocked)
	{
		// 잠긴 소리 재생
		if (LockedSound)
		{
			UGameplayStatics::PlaySoundAtLocation(this, LockedSound, GetActorLocation());
		}

		UE_LOG(LogTemp, Warning, TEXT("Door is Locked. Cannot open."));

		// 부모의 Grab(문 열기)을 호출하지 않고 여기서 종료
		return;
	}

	// 2. 잠겨있지 않다면 부모 클래스의 원래 기능(문 열기) 실행
	Super::Grab_Implementation(HandController);
}

void AGimmickDoor::UnlockDoor(bool bOpenImmediately)
{
	if (!bIsLocked) return; // 이미 열려있다면 무시

	bIsLocked = false;
	UE_LOG(LogTemp, Warning, TEXT("Door Unlocked!"));

	// 잠금 해제 사운드 재생
	if (UnlockSound)
	{
		UGameplayStatics::PlaySoundAtLocation(this, UnlockSound, GetActorLocation());
	}

	// 옵션: 해제와 동시에 문 열기
	if (bOpenImmediately)
	{
		OpenDoor();
	}
}

void AGimmickDoor::LockDoor()
{
	if (bIsLocked) return;

	bIsLocked = true;

	// 잠글 때는 문을 닫아줍니다 (열린 채로 잠기면 이상하므로)
	CloseDoor();

	UE_LOG(LogTemp, Warning, TEXT("Door Locked!"));
}