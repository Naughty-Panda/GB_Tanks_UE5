// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Pawn.h"
#include "Cannon.h"
#include "DamageTaker.h"

#include "TankBase.generated.h"

class UArrowComponent;
class UBoxComponent;
class USphereComponent;
class UHealthComponent;
class UParticleSystem;
class USoundBase;
class UWidgetComponent;

UCLASS()
class GB_TANKS_UE5_API ATankBase : public APawn, public IDamageTaker
{
	GENERATED_BODY()

public:
	/** Main collider for the turret */
	UPROPERTY(VisibleDefaultsOnly, BlueprintReadWrite, Category = "Components")
	UBoxComponent* Collider;

	UPROPERTY(VisibleDefaultsOnly, BlueprintReadWrite, Category = "Components")
	USphereComponent* VisibilityRange;

	/** Body mesh component */
	UPROPERTY(VisibleDefaultsOnly, BlueprintReadWrite, Category = "Components")
	UStaticMeshComponent* BodyMesh;

	/** Turret body mesh component */
	UPROPERTY(VisibleDefaultsOnly, BlueprintReadWrite, Category = "Components")
	UStaticMeshComponent* TurretMesh;

	UPROPERTY(VisibleDefaultsOnly, BlueprintReadWrite, Category = "Components")
	UArrowComponent* CannonAttachPoint;

	/** Default class for tank's cannon */
	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "Components")
	TSubclassOf<ACannon> DefaultCannonClass;

	/** Component that holds widget for tank's Health Bar */
	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "UI")
	UWidgetComponent* HealthBarComponent;

	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "VFX")
	UParticleSystem* DestructionParticleEffect;

	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "VFX")
	USoundBase* DestructionSound;

	/** Tank's health component */
	UPROPERTY(VisibleDefaultsOnly, BlueprintReadWrite, Category = "Components")
	UHealthComponent* HealthComponent;

	/** Tank move speed */
	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "Movement|Speed")
	float MoveSpeed = 500.0f;

	/** Tank rotation speed */
	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "Movement|Speed")
	float RotationSpeed = 100.0f;

	/** Tank rotation interpolation speed */
	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "Movement|Speed")
	float BaseRotInterpSpeed = 1.5f;

	/** Turret rotation interpolation speed */
	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "Movement|Speed")
	float TurrerRotInterpSpeed = 150.0f;

protected:
	/** Cannons */
	UPROPERTY()
	ACannon* PrimaryCannon;
	UPROPERTY()
	ACannon* SecondaryCannon;

	/** Targeting params for AI controlled pawns */
	UPROPERTY()
	bool bIsAIControlled = false;

	bool bIsTargetInRange = false;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "AI|Targeting", meta = (EditCondition = "bIsAIControlled", EditConditionHides))
	float TargetingRange = 3000.0f;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "AI|Targeting", meta = (EditCondition = "bIsAIControlled", EditConditionHides))
	float TargetingRate = 0.05f;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "AI|Targeting", meta = (EditCondition = "bIsAIControlled", EditConditionHides))
	float TargetingAccuracy = 10.0f;

	/** Holds forward, right and rotation axes provided by user input */
	float TargetForwardAxisValue = 0.0f;
	float TargetRightAxisValue = 0.0f;
	float TargetRotationAxisValue = 0.0f;
	float CurrentRotationAxisValue = 0.0f;

public:
	// Sets default values for this pawn's properties
	ATankBase();

	void MoveForward(float AxisValue);
	void MoveRight(float AxisValue);
	void Rotate(float AxisValue);
	bool IsAimedAtLocation(const FVector& TargetLocation) const;
	// bool IsTargetInRange(const APawn* Target) const;
	bool HasLineOfSightOnTarget(const APawn* Target) const;
	void LockOnTarget(const APawn* Target);
	void Fire(ECannonFireMode FireMode);
	void SwitchCannon();
	void SetupCannon(TSubclassOf<ACannon> InCannon);
	ACannon* GetPrimaryCannon() const;
	ACannon* GetSecondaryCannon() const;
	bool IsAIControlled() const { return bIsAIControlled; }
	virtual void TakeDamage(const FDamageData& DamageData) override;

	UFUNCTION(BlueprintCallable, Category = "Turret")
	FVector GetTurretForwardVector() const;

	UFUNCTION(BlueprintCallable, Category = "Turret")
	void RotateTurretTo(const FVector& TargetPosition);

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;
	virtual void Destroyed() override;

	UFUNCTION()
	void Die();

	UFUNCTION()
	virtual void DamageTaken(float DamageValue);

	UFUNCTION()
	virtual void OnHealthChanged(float MaxHealth, float CurrentHealth);

	UFUNCTION()
	void OnEnteringVisibilityRange(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor,
	                               UPrimitiveComponent* OtherComp, int32 OtherBodyIndex,
	                               bool bFromSweep, const FHitResult& SweepResult);
	UFUNCTION()
	void OnLeavingVisibilityRange(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor,
	                              UPrimitiveComponent* OtherComp, int32 OtherBodyIndex);

public:
	// Called every frame
	virtual void Tick(float DeltaTime) override;
};
