#include "MainClass/Characters/Boss/Boss.h"
#include "MainClass/Characters/Boss/BossAIController.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "Components/CapsuleComponent.h"
#include "Animation/AnimInstance.h"
#include "BehaviorTree/BlackboardComponent.h"
//#include "BossAIController.h"
#include "ThrownItem.h"

const FName ABoss::StunKey = TEXT("IsStunned");

// Sets default values
ABoss::ABoss()
{
	// Tick은 켜두되, 이동 로직은 넣지 않습니다.
	PrimaryActorTick.bCanEverTick = true;

	// AI 컨트롤러가 자동으로 빙의되도록 설정
	AutoPossessAI = EAutoPossessAI::PlacedInWorldOrSpawned;

	// 보스 회전 설정 - 컨트롤러가 회전을 제어하도록
	bUseControllerRotationYaw = false;
	GetCharacterMovement()->bOrientRotationToMovement = true;
	GetCharacterMovement()->RotationRate = FRotator(0.0f, 500.0f, 0.0f);
}

// Called when the game starts or when spawned
void ABoss::BeginPlay()
{
	Super::BeginPlay();

	// AI Controller 연결 확인 (디버깅용)
	ABossAIController* BossAI = Cast<ABossAIController>(GetController());
	if (!BossAI)
	{
		UE_LOG(LogTemp, Error, TEXT("Boss: AI Controller NOT Found!"));
	}
	else
	{
		UE_LOG(LogTemp, Warning, TEXT("Boss: AI Controller Connected. Logic is handled by BT."));
	}
}

// Called every frame
void ABoss::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
}

// Called to bind functionality to input
void ABoss::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);
}

// Behavior Tree에서 호출할 공격 함수
void ABoss::Attack()
{
	// [수정 1] 이미 공격 중이라면 명령 무시 (애니메이션 무한 리셋 방지)
	if (bIsAttacking)
	{
		UE_LOG(LogTemp, Error, TEXT("Boss: Boss is Already Attacking")); 
		bIsAttacking = false; //for quick fix
		return;
	}
		

	if (AttackMontage)
	{
		UAnimInstance* AnimInstance = GetMesh()->GetAnimInstance();
		if (AnimInstance)
		{
			// [수정 2] 공격 시작 시 미끄러짐 방지 (즉시 정지)
			GetCharacterMovement()->StopMovementImmediately();

			// 공격 상태 설정
			bIsAttacking = true;

			// 몽타주 재생
			AnimInstance->Montage_Play(AttackMontage);

			bIsAttacking = false;

			// 몽타주 종료 시점 바인딩
			FOnMontageEnded EndDelegate;
			EndDelegate.BindUObject(this, &ABoss::OnAttackMontageEnded);
			AnimInstance->Montage_SetEndDelegate(EndDelegate, AttackMontage);

			UE_LOG(LogTemp, Warning, TEXT("Boss: Attacking Player!"));
		}
	}
}

void ABoss::Interact_Implementation(AActor* Interactor)
{
	AActor* Item = Cast<AActor>(Interactor);
	if (!Item->ActorHasTag("Projectile")) return;
	ABossAIController* AIController = Cast<ABossAIController>(GetController());
	if (!AIController) return;
	UBlackboardComponent* BlackboardComp = AIController->GetBlackboardComponent();
	if (!BlackboardComp) return;

	UE_LOG(LogTemp, Warning, TEXT("Boss: Boss is hit by Item!"));
	BlackboardComp->SetValueAsBool(StunKey, true);
}

void ABoss::KillBoss()
{
	if (bIsDead) return;
	UE_LOG(LogTemp, Warning, TEXT("%s is killed!"), *GetName());
	OnDeath();
}

void ABoss::OnAttackMontageEnded(UAnimMontage* Montage, bool bInterrupted)
{
	// [수정 3] 공격 종료 시 상태 해제 (다시 이동/공격 가능)
	bIsAttacking = false;

	UE_LOG(LogTemp, Warning, TEXT("Boss: Attack Finished."));
}

float ABoss::TakeDamage(float DamageAmount, FDamageEvent const& DamageEvent, AController* EventInstigator, AActor* DamageCauser)
{
	const float ActualDamage = Super::TakeDamage(DamageAmount, DamageEvent, EventInstigator, DamageCauser);

	if (ActualDamage > 0)
	{
		Health -= ActualDamage;

		if (Health <= 0) 
		{
			Health = 0;
			OnDeath();
		}
	}
	return ActualDamage;
}

void ABoss::OnDeath()
{
	if (bIsDead) return;
	ABossAIController* AIController = Cast<ABossAIController>(GetController());
	AIController->GetBrainComponent()->StopLogic("Boss Died");
	AIController->ClearFocus(EAIFocusPriority::Gameplay);
	StopAnimMontage();
	GetCharacterMovement()->DisableMovement();
	bIsDead = true; //Flag -> set  animation in ABP
	HandleDeath();
}
