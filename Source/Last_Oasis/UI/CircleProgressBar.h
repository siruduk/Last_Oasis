// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "CircleProgressBar.generated.h"

/**
 * 
 */
UCLASS()
class LAST_OASIS_API UCircleProgressBar : public UUserWidget
{
	GENERATED_BODY()
	virtual void NativeConstruct() override;
	
	UFUNCTION(BlueprintCallable)
	void SetPercent();
	
	UPROPERTY(EditAnywhere,Meta=(AllowPrivateAccess = true))
	TObjectPtr<class UMaterialInstance> ProgressBarInstance;
	UPROPERTY(EditAnywhere,Meta=(AllowPrivateAccess = true))
	TObjectPtr<class UMaterialInstanceDynamic> DynamicProgressBarInstance;
	
	UPROPERTY(Meta=(BindWidget, AllowPrivateAccess = true),BlueprintReadWrite)
	TObjectPtr<class UImage> ProgressBar;
	UPROPERTY(EditAnywhere,Meta=(AllowPrivateAccess = true))
	float Value;
};
