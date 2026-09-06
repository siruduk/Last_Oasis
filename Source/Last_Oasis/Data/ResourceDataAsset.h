// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Engine/DataAsset.h"
#include "../Data/DataAssetBase.h"
#include "ResourceDataAsset.generated.h"

/**
 * 
 */
UCLASS(BlueprintType, Blueprintable)
class LAST_OASIS_API UResourceDataAsset : public UPrimaryDataAsset
{
	GENERATED_BODY()
public:

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Resource")
	UDataAssetBase* ItemData;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Resource")
	int32 NeedCount;

};
