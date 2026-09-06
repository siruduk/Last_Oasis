// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "Components/TextBlock.h"
#include "Components/Image.h"
#include "ItemInfoWidget.generated.h"

class UDataAssetBase;
/**
 * 
 */
UCLASS()
class LAST_OASIS_API UItemInfoWidget : public UUserWidget
{
	GENERATED_BODY()
public:
	UPROPERTY(meta = (BindWidget))
	class UImage* ItemIcon;

	UPROPERTY(meta = (BindWidget))
	class UTextBlock* ItemName;

	UPROPERTY(meta = (BindWidget))
	class UTextBlock* ItemDescription;

	UPROPERTY(meta = (BindWidget))
	class UTextBlock* ItemType;

	void SetItemData(UDataAssetBase* ItemData) const;
	
};
