// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "GB_Tanks_UE5/GameStructs.h"

#include "HealthComponent.generated.h"

UCLASS(ClassGroup=(Custom), meta=(BlueprintSpawnableComponent))
class GB_TANKS_UE5_API UHealthComponent : public UActorComponent
{
	GENERATED_BODY()

	DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FOnHealthChanged, float, DamageValue);

	DECLARE_DYNAMIC_MULTICAST_DELEGATE(FOnDie);

public:
	UPROPERTY(BlueprintAssignable)
	FOnHealthChanged OnDamaged;

	UPROPERTY(BlueprintAssignable)
	FOnDie OnDie;

protected:
	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "Health")
	float MaxHealth = 10.0f;

	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "Health")
	float CurrentHealth = MaxHealth;

public:
	// Sets default values for this component's properties
	UHealthComponent();

	void TakeDamage(const FDamageData& DamageData);
	float GetHealth() const;
	float GetHealthState() const;
	void AddHealth(float HealthValue);

protected:
	// Called when the game starts
	virtual void BeginPlay() override;

	// Called every frame
	virtual void TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction) override;
};
