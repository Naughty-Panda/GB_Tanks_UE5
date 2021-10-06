// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "MapLoader.generated.h"

class UStaticMeshComponent;
class UBoxComponent;
class UPointLightComponent;

UCLASS()
class GB_TANKS_UE5_API AMapLoader : public AActor
{
	GENERATED_BODY()

protected:
	UPROPERTY(VisibleDefaultsOnly, BlueprintReadWrite, Category="Components")
	UStaticMeshComponent* GateMesh;

	UPROPERTY(VisibleDefaultsOnly, BlueprintReadWrite, Category="Components")
	UBoxComponent* TriggerCollider;

	UPROPERTY(VisibleDefaultsOnly, BlueprintReadWrite, Category="Components")
	UPointLightComponent* StatusLight;

	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category="Components")
	FLinearColor ActiveColor;

	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category="Components")
	FLinearColor InactiveColor;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="Level params")
	FName NextLevelName;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="Level params")
	bool bIsActivated = false;


public:
	// Sets default values for this actor's properties
	AMapLoader();

	void SetActivated(bool InActivated);
	void SwitchStatusLight();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

	UFUNCTION()
	void OnTriggerBeginOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp,
	                           int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult);
};
