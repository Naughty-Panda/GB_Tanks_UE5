// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "DamageTaker.h"
#include "GameFramework/Actor.h"

#include "TankFactory.generated.h"

class ATankBot;
class UStaticMeshComponent;
class UArrowComponent;
class UBoxComponent;
class ATargetPoint;
class UHealthComponent;
class UParticleSystem;
class USoundBase;
class AMapLoader;

UCLASS()
class GB_TANKS_UE5_API ATankFactory : public AActor, public IDamageTaker
{
	GENERATED_BODY()

protected:
	UPROPERTY(VisibleDefaultsOnly, BlueprintReadWrite, Category="Components")
	UStaticMeshComponent* BuildingMesh;

	UPROPERTY(VisibleDefaultsOnly, BlueprintReadWrite, Category="Components")
	UStaticMeshComponent* DestroyedMesh;

	UPROPERTY(VisibleDefaultsOnly, BlueprintReadWrite, Category="Components")
	UBoxComponent* Collider;

	UPROPERTY(VisibleDefaultsOnly, BlueprintReadWrite, Category="Components")
	UArrowComponent* TankSpawningPoint;

	UPROPERTY(VisibleDefaultsOnly, BlueprintReadWrite, Category="Components")
	UHealthComponent* HealthComponent;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Components")
	AMapLoader* LinkedMapLoader;

	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "VFX")
	UParticleSystem* DestructionParticleEffect;

	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "VFX")
	USoundBase* DestructionSound;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="Spawn Params")
	TSubclassOf<ATankBot> DefaultTankSpawnClass;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="Spawn Params")
	TArray<ATargetPoint*> TankWaypoint;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="Spawn Params")
	float SpawnRate = 10.f;

	FTimerHandle SpawnTimerHandle;
	FTimerDelegate SpawnDelegate;


public:
	// Sets default values for this actor's properties
	ATankFactory();

	UFUNCTION()
	virtual void TakeDamage(const FDamageData& DamageData) override;

	UFUNCTION()
	void SpawnNewTank();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;
	virtual void Destroyed() override;

	UFUNCTION()
	void Die();

	UFUNCTION()
	void DamageTaken(float DamageValue);
};
