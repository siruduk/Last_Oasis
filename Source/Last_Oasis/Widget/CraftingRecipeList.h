// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "Components/TextBlock.h"
#include "Components/Image.h"
#include "Components/Border.h"
#include "CraftingRecipeList.generated.h"

class UDataAssetBase;
class UCraftingWidget;
/**
 * 
 */
UCLASS()
class LAST_OASIS_API UCraftingRecipeList : public UUserWidget
{
	GENERATED_BODY()
	
public:
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Crafting")
	class UCraftingWidget* ParentCraftingWidget;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Crafting")
	UDataAssetBase* ItemData;

	UPROPERTY(meta = (BindWidget))
	class UTextBlock* CraftingItemName;

	UPROPERTY(meta = (BindWidget))
	class UImage* CraftingItemImage;

	UPROPERTY(meta = (BindWidget))
	UBorder* BackgroundBorder;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Crafting")
	bool bIsSelected = false;

	virtual void NativeConstruct() override;

	UFUNCTION()
	void OnClicked();

	void SetSelected(bool bSelected);

	FLinearColor DefaultBrushColor;

	UFUNCTION(BlueprintCallable, Category = "Crafting")
	void SetRecipeData();
};
