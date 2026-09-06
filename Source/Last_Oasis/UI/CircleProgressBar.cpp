// Fill out your copyright notice in the Description page of Project Settings.


#include "UI/CircleProgressBar.h"
#include "Components/Image.h"
#include "Kismet/BlueprintFunctionLibrary.h"


void UCircleProgressBar::NativeConstruct()
{
	Super::NativeConstruct();
	SetPercent();
}

void UCircleProgressBar::SetPercent()
{
	if (!IsValid(DynamicProgressBarInstance))
	{
		DynamicProgressBarInstance = UMaterialInstanceDynamic::Create(ProgressBarInstance,this);
		ProgressBar->SetBrushFromMaterial(DynamicProgressBarInstance);
	}
	DynamicProgressBarInstance->SetScalarParameterValue("Percent",Value);
}
