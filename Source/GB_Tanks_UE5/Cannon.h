// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "GameStructs.h"

#include "Cannon.generated.h"

class UStaticMeshComponent;
class UArrowComponent;
class AProjectile;
class UParticleSystemComponent;
class UAudioComponent;
class UCameraShakeBase;
class UForceFeedbackEffect;

UCLASS()
class GB_TANKS_UE5_API ACannon : public AActor
{
	GENERATED_BODY()

protected:
	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "Components")
	UStaticMeshComponent* CannonMesh;

	UPROPERTY(VisibleDefaultsOnly, BlueprintReadWrite, Category = "Components")
	UArrowComponent* ProjectileSpawnPoint;

	/** Current cannon type */
	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "Fire settings")
	ECannonType CannonType = ECannonType::TraceCannon;

	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, meta = (EditCondition = "CannonType == ECannonType::ProjectileCannon", EditConditionHides),
		Category = "Fire settings")
	TSubclassOf<AProjectile> DefaultProjectileClass;

	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "Ammo")
	FCannonAmmo Ammo;
  
	/**
	 *  Shot VFX
	 */

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "VFX", meta = (EditCondition = "CannonType == ECannonType::ProjectileCannon", EditConditionHides))
	UParticleSystemComponent* ProjectileShootEffect;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "VFX", meta = (EditCondition = "CannonType == ECannonType::TarceCannon", EditConditionHides))
	UParticleSystemComponent* TraceShootEffect;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "VFX", meta = (EditCondition = "CannonType == ECannonType::ProjectileCannon", EditConditionHides))
	UAudioComponent* ProjectileAudioEffect;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "VFX", meta = (EditCondition = "CannonType == ECannonType::TraceCannon", EditConditionHides))
	UAudioComponent* TraceAudioEffect;

	/**
	 * Shot feedback
	 */

	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "Shot Feedback")
	TSubclassOf<UCameraShakeBase> CameraShake;

	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "Shot Feedback")
	UForceFeedbackEffect* ForceFeedbackEffect;

	/**
	* Fire Settings
	*/

	/** Number of shots in burst fire mode */
	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "Fire settings")
	uint8 BurstFireShots = 3;

	/** Number of shots to be fired in burst fire mode */
	uint8 BurstFireShotsLeft = BurstFireShots;

	/** Rate of fire in burst fire mode */
	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, meta = (EditCondition = "BurstFireShots > 1", EditConditionHides), Category = "Fire settings")
	float FireRateBurst = 1.0f;
  
	//** Number of shots in burst fire mode */
	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "Fire settings")
	uint8 BurstFireShots = 3;

	//** Number of shots to be fired in burst fire mode */
	uint8 BurstFireShotsLeft = BurstFireShots;

	//** Rate of fire in burst fire mode */
	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, meta = (EditCondition = "BurstFireShots > 1", EditConditionHides), Category = "Fire settings")
	float FireRateBurst = 1.0f;

	//** Fire rate in single shot mode */
	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "Fire settings")
	float FireRateSingle = 1.0f;

	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "Fire settings")
	float FireRange = 1000.0f;

	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "Fire settings")
	float FireDamage = 1.0f;

	bool bReadyToFire = true;

	/** Cannon timers */
	FTimerHandle ReloadTimerHandle;
	FTimerHandle BurstFireTimerHandle;

public:
	// Sets default values for this actor's properties
	ACannon();

	void Fire(ECannonFireMode FireMode);
	bool FORCEINLINE IsReadyToFire() const { return bReadyToFire && Ammo.HasAmmo(); }
	void FORCEINLINE AddAmmo(uint8 Amount) { Ammo.AddAmmo(Amount); }
	bool FORCEINLINE HasFullAmmo() const { return Ammo.HasFullAmmo(); }

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;
	virtual void EndPlay(const EEndPlayReason::Type EndPlayReason) override;

	void FireSingle();
	void FireBurst();
	void Reload();
	void ShootProjectile();
	void ShootTrace();

	void PlayForceFeedback() const;
	void PlayCameraShake() const;
};
