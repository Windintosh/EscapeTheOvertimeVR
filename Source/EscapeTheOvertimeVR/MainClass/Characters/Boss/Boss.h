#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Character.h"
#include "ThrownItem.h"
#include "Boss.generated.h" 

// 포워드 선언
class ABossAIController;
class UAnimMontage;

UCLASS()
class ESCAPETHEOVERTIMEVR_API ABoss : public ACharacter, public IBossHit
{
	GENERATED_BODY()

public:
	// Sets default values for this character's properties
	ABoss();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;


public:
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	// Called to bind functionality to input
	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;

	/* ================================================== */
	/* 이동은 AIController가 하므로, 공격 관련 기능만 추가  */
	/* ================================================== */

	/** 에디터에서 할당할 공격 애니메이션 몽타주 */
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Combat")
	UAnimMontage* AttackMontage;

	/** Behavior Tree(Task)에서 호출할 공격 함수 */
	UFUNCTION(BlueprintCallable, Category = "Combat")
	virtual void Attack(); // 자식에서 확장 가능하도록 virtual 유지

	UFUNCTION(BlueprintNativeEvent, BlueprintCallable, Category = "Interaction")
	void Interact(AActor* Interactor);
	virtual void Interact_Implementation(AActor* Interactor) override;

	static const FName StunKey;

	// [추가] 공격 중복 실행 방지용 플래그
	UPROPERTY(BlueprintReadWrite)
	bool bIsAttacking = false;

protected: // (private에서 protected로 변경 권장: 자식 클래스에서도 접근 가능하게)
	/** 몽타주 재생이 끝났을 때 호출되는 콜백 함수 */
	UFUNCTION()
	virtual void OnAttackMontageEnded(UAnimMontage* Montage, bool bInterrupted);

	virtual float TakeDamage(float DamageAmount, struct FDamageEvent const& DamageEvent, AController* EventInstigator, AActor* DamageCauser) override;
	
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Boss Stats")
	float Health = 300.f;

	virtual void OnDeath();

	UFUNCTION(BlueprintImplementableEvent, Category = "Boss Stats")
	void HandleDeath();
	
	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = "Boss Stats")
	bool bIsDead = false;

};
