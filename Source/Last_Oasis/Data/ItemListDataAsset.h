// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Engine/DataAsset.h"
#include "../Data/DataAssetBase.h"
#include "ItemListDataAsset.generated.h"

/**
 * 
 */
UCLASS()
class LAST_OASIS_API UItemListDataAsset : public UPrimaryDataAsset
{
	GENERATED_BODY()

public:
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Inventory")
	TArray<UDataAssetBase*> itemList;
};
