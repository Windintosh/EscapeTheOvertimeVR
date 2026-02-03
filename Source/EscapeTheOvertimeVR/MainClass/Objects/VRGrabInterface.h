#pragma once

#include "CoreMinimal.h"
#include "UObject/Interface.h"
#include "VRGrabInterface.generated.h"

class USceneComponent;

// Blueprintable: 이 인터페이스를 블루프린트 전용 클래스(예: BP_Pistol)도 상속받을 수 있게 함
UINTERFACE(MinimalAPI, Blueprintable)
class UVRGrabInterface : public UInterface
{
	GENERATED_BODY()
};

/**
 * 
 */
class ESCAPETHEOVERTIMEVR_API IVRGrabInterface
{
	GENERATED_BODY()

	// Add interface functions to this class. This is the class that will be inherited to implement this interface.
public:

	// BlueprintNativeEvent: C++과 BP 양쪽에서 구현 가능
	// 파라미터 HandController: 잡은 주체(손의 MotionController 또는 Mesh)를 넘겨줘서 AttachToComponent에 활용

	UFUNCTION(BlueprintCallable, BlueprintNativeEvent, Category = "VR Interaction")
	void Grab(USceneComponent* HandController);

	UFUNCTION(BlueprintCallable, BlueprintNativeEvent, Category = "VR Interaction")
	void Release(FVector ThrowVelocity);

	// [추가] 쥐고 있는 상태에서 트리거 버튼을 눌렀을 때 호출
	// Value: 0.0 ~ 1.0 (아날로그 트리거 압력)
	UFUNCTION(BlueprintCallable, BlueprintNativeEvent, Category = "VR Interaction")
	void OnAction(float Value);

	bool bIsSpawnedItem = false;
};
