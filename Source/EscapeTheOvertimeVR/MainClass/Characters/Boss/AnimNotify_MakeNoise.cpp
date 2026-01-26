#include "AnimNotify_MakeNoise.h"
#include "Components/SkeletalMeshComponent.h"
#include "GameFramework/Actor.h"
#include "GameFramework/Pawn.h"

UAnimNotify_MakeNoise::UAnimNotify_MakeNoise()
{
    // 기본값 설정
    Loudness = 1.0f;
    MaxRange = 0.0f;

    // 에디터에서 노티파이 색상 변경 (눈에 잘 띄게)
    bIsNativeBranchingPoint = false;
}

FString UAnimNotify_MakeNoise::GetNotifyName_Implementation() const
{
    return TEXT("Make AI Noise");
}

void UAnimNotify_MakeNoise::Notify(USkeletalMeshComponent* MeshComp, UAnimSequenceBase* Animation, const FAnimNotifyEventReference& EventReference)
{
    Super::Notify(MeshComp, Animation, EventReference);

    if (MeshComp && MeshComp->GetOwner())
    {
        AActor* OwnerActor = MeshComp->GetOwner();

        // 1. 소음을 발생시킴 (Pawn이어야 MakeNoise가 제대로 작동함)
        if (APawn* OwnerPawn = Cast<APawn>(OwnerActor))
        {
            // MakeNoise(소리크기, 소음유발자, 소음위치, 최대거리)
            OwnerPawn->MakeNoise(Loudness, OwnerPawn, OwnerPawn->GetActorLocation(), MaxRange);
            UE_LOG(LogTemp, Warning, TEXT("MakeNoise: Noise Created!"));
        }
    }
}
