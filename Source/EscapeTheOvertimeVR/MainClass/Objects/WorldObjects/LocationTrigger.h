// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "Components/BoxComponent.h"
#include "Variant_Horror/HorrorCharacter.h" // Enum(EDeathLocationType) 인식을 위해 포함
#include "LocationTrigger.generated.h"

UCLASS()
class ESCAPETHEOVERTIMEVR_API ALocationTrigger : public AActor
{
	GENERATED_BODY()

public:
	// Sets default values for this actor's properties
	ALocationTrigger();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:
	// Called every frame
	virtual void Tick(float DeltaTime) override;

protected:
	// 충돌 감지를 위한 박스 컴포넌트
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Components")
	UBoxComponent* TriggerBox;

	// 에디터에서 설정할 이 구역의 장소 태그 (사무실, 창가 등)
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Location")
	EDeathLocationType LocationType;

	// 오버랩 시작 시 호출될 함수
	UFUNCTION()
	void OnOverlapBegin(UPrimitiveComponent* OverlappedComp, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult);
};
