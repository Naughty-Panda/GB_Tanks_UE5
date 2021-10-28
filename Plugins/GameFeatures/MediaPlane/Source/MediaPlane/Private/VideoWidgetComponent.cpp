// Fill out your copyright notice in the Description page of Project Settings.

#include "VideoWidgetComponent.h"

void UVideoWidgetComponent::BeginPlay()
{
	Super::BeginPlay();

	AttachToComponent(GetOwner()->GetRootComponent(), FAttachmentTransformRules::KeepRelativeTransform);
	SetRelativeLocation({0.f, 0.f, 450.f});
	SetRelativeRotation({-270.f, 180.f, 0.f});
}
