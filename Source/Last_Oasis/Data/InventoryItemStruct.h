// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "../Data/DataAssetBase.h"
#include "InventoryItemStruct.generated.h"


USTRUCT(BlueprintType, Blueprintable)
struct FInventoryItem
{
	GENERATED_BODY()

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Inventory")
	UDataAssetBase* ItemData;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Inventory")
	int32 CurrentCount;

};