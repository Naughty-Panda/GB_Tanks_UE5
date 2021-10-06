// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "Projectile.generated.h"

UCLASS()
class GB_TANKS_UE5_API AProjectile : public AActor
{
	GENERATED_BODY()

protected:
	UPROPERTY(VisibleDefaultsOnly, BlueprintReadWrite, Category="Components")
	UStaticMeshComponent* Mesh;

	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category="Movement|Speed")
	float MoveSpeed = 100.0f;

	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category="Movement|Speed")
	float FlyRange = 10000.0f;

	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category="Movement|Speed")
	float MoveRate = 0.5f;

	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category="Damage")
	float Damage = 1.0f;

	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category="Damage")
	float PushImpulse = 1000.0f;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="Explosion")
	bool bCanExplode = true;

	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category="Explosion", meta = (EditCondition = "bCanExplode", EditConditionHides))
	float ExplodeRadius = 50.0f;
  
private:
	FTimerHandle MovementTimerHandle;

public:
	// Sets default values for this actor's properties
	AProjectile();

	virtual void Start();
	virtual void Explode();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;
	virtual void EndPlay(const EEndPlayReason::Type EndPlayReason) override;

	UFUNCTION()
	void OnMeshBeginOverlap(
		UPrimitiveComponent* OverlappedComponent,
		AActor* OtherActor,
		UPrimitiveComponent* OtherComponent,
		int32 OtherBodyIndex,
		bool bFromSweep,
		const FHitResult& HitResult
	);
  
	virtual void Move();
};
