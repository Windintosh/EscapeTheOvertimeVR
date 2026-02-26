#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "Components/SphereComponent.h"
#include "EnemyProjectile.generated.h"

UCLASS()
class ESCAPETHEOVERTIMEVR_API AEnemyProjectile : public AActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	AEnemyProjectile();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Item")
	USceneComponent* Scene;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Item")
	USphereComponent* Collision;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Item")
	UStaticMeshComponent* StaticMesh;

	UFUNCTION()
	virtual void OnPlayerOverlap(UPrimitiveComponent* OverlappedComp, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult);

	void DestroyProjectile();

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Projectile")
	float Speed = 800.f;

	float Damage = 20.f;

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

};
