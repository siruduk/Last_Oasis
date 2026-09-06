// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "../Data/DataAssetBase.h"
#include "../Data/ResourceDataAsset.h"
#include "RecipeState.generated.h"

USTRUCT(BlueprintType)
struct FRecipeResource
{
    GENERATED_BODY()

    UPROPERTY(EditAnywhere, BlueprintReadWrite)
    UDataAssetBase* Resource; // 재료 정보

    UPROPERTY(EditAnywhere, BlueprintReadWrite)
    int32 NeedCount; // 필요한 개수
};

USTRUCT(BlueprintType)
struct FRecipeState
{
    GENERATED_BODY()

public:
    UPROPERTY(EditAnywhere, BlueprintReadWrite)
	UDataAssetBase* RecipeItem; // 제작 아이템

    UPROPERTY(EditAnywhere, BlueprintReadWrite)
	TArray<FRecipeResource> RequiredResources; // 필요한 자원 
    
    UPROPERTY(EditAnywhere, BlueprintReadWrite)
    bool bUnlocked = false; // 잠금 해제 여부

    UPROPERTY(EditAnywhere, BlueprintReadWrite)
    bool bCrafted = false;  // 이미 제작했는지 여부

    UPROPERTY(EditAnywhere, BlueprintReadWrite)
    bool bAvailable = true; // 현재 제작 가능한지 여부
};