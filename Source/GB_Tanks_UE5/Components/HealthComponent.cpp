// Fill out your copyright notice in the Description page of Project Settings.

#include "HealthComponent.h"

#include "GB_Tanks_UE5/GB_Tanks_UE5.h"

// Sets default values for this component's properties
UHealthComponent::UHealthComponent()
{
	// Set this component to be initialized when the game starts, and to be ticked every frame.  You can turn these features
	// off to improve performance if you don't need them.
	PrimaryComponentTick.bCanEverTick = false;

	bAutoActivate = true;
}

void UHealthComponent::TakeDamage(const FDamageData& DamageData)
{
	const float TakenDamage = DamageData.DamageValue;
	CurrentHealth -= TakenDamage;

	if (CurrentHealth < 0.0f)
	{
		if (OnDie.IsBound())
		{
			OnDie.Broadcast();
		}
	}
	else
	{
		if (OnDamaged.IsBound())
		{
			OnDamaged.Broadcast(TakenDamage);
		}
	}
}

float UHealthComponent::GetHealth() const
{
	return CurrentHealth;
}

float UHealthComponent::GetHealthState() const
{
	return CurrentHealth / MaxHealth;
}

void UHealthComponent::AddHealth(float HealthValue)
{
	CurrentHealth = FMath::Clamp(CurrentHealth + HealthValue, 0.0f, MaxHealth);
}


// Called when the game starts
void UHealthComponent::BeginPlay()
{
	Super::BeginPlay();

	// ...
}


// Called every frame
void UHealthComponent::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);

	UE_LOG(LogTanks, Warning, TEXT("ComponentTick..."));
}
