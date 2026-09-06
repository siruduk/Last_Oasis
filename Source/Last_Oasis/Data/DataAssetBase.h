// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Engine/DataAsset.h"
#include "DataAssetBase.generated.h"

/**
 * 
 */
UCLASS(BlueprintType, Blueprintable)
class LAST_OASIS_API UDataAssetBase : public UPrimaryDataAsset
{
	GENERATED_BODY()
	
public:
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Data")
	int32 ItemIndex;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Data")
	FName ItemName;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Data")
	FText Description;

	UPROPERTY(EditAnywhere, BlueprintReadOnly) 
	UTexture2D* Icon;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Data")
	bool IsUsable; // Player can use

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Data")
	bool IsCombinable; // Use for Crafting

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Data")
	bool IsStackable; // stack of inventory

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Data")
	int32 MaxStackCount; // Max of stack inventory

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Data")
	float UseValue; // get value when use
};
