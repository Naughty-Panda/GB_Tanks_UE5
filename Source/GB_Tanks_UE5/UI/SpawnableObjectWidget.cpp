// Fill out your copyright notice in the Description page of Project Settings.

#include "SpawnableObjectWidget.h"
#include "Components/TextBlock.h"
#include "Kismet/GameplayStatics.h"
#include "../TankPlayerController.h"

void USpawnableObjectWidget::NativeTick(const FGeometry& MyGeometry, float InDeltaTime)
{
	Super::NativeTick(MyGeometry, InDeltaTime);

	if (SpawnedActor)
	{
		if (PlayerController)
		{
			FVector WorldPosition, WorldDirection;
			PlayerController->DeprojectMousePositionToWorld(WorldPosition, WorldDirection);

			FHitResult HitResult;
			FCollisionQueryParams CollisionParams;
			CollisionParams.AddIgnoredActor(PlayerController->GetPawn());
			CollisionParams.AddIgnoredActor(SpawnedActor);

			GetWorld()->LineTraceSingleByChannel(HitResult, WorldPosition, WorldPosition + WorldDirection * 100'000,
			                                     ECollisionChannel::ECC_WorldStatic, CollisionParams);

			if (HitResult.GetActor())
			{
				SpawnedActor->SetActorLocation(HitResult.Location);
			}
		}
	}
}

void USpawnableObjectWidget::NativePreConstruct()
{
	Super::NativePreConstruct();

	if (SpawnableObjectNameTextBlock)
	{
		SpawnableObjectNameTextBlock->SetText(SpawnableObjectName);
	}
}

void USpawnableObjectWidget::NativeConstruct()
{
	Super::NativeConstruct();

	PlayerController = Cast<ATankPlayerController>(UGameplayStatics::GetPlayerController(GetWorld(), 0));

	if (PlayerController)
	{
		// Binding to this delegate should be on NativeConstruct because PlayerController doesn't exist on NativePreConstruct.
		PlayerController->OnLeftMouseButtonUpDelegate.AddUObject(this, &USpawnableObjectWidget::OnMouseButtonUp);
	}

	// Trying to create tooltip...
	// Doesn't work...
	if (TooltipClass)
	{
		if (UUserWidget* NewTooltip = CreateWidget(GetWorld(), TooltipClass))
		{
			SetToolTip(NewTooltip);
		}
	}

	SetToolTipText(SpawnableObjectName);
}

FReply USpawnableObjectWidget::NativeOnMouseButtonDown(const FGeometry& InGeometry, const FPointerEvent& InMouseEvent)
{
	if (InMouseEvent.IsMouseButtonDown(EKeys::LeftMouseButton))
	{
		SpawnedActor = GetWorld()->SpawnActor(SpawnableClass);
	}

	return FReply::Handled();
}

void USpawnableObjectWidget::OnMouseButtonUp()
{
	SpawnedActor = nullptr;
}
