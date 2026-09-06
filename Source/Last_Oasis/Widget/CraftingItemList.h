// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "Components/TextBlock.h"
#include "Components/Image.h"
#include "../Data/RecipeState.h"
#include "CraftingItemList.generated.h"

class UDataAssetBase;
class UCraftingWidget;
class ACraftingManager;
/**
 * 
 */
UCLASS()
class LAST_OASIS_API UCraftingItemList : public UUserWidget
{
	GENERATED_BODY()
public:
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Crafting")
	class UCraftingWidget* ParentCraftingWidget;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Crafting")
	ACraftingManager* CraftingManager;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Crafting")
	FRecipeResource ResourceData;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Crafting")
	int32 InventoryItemCount;

	bool bIsSufficientResource = false;

	UPROPERTY(meta = (BindWidget))
	class UTextBlock* CraftingResourceName;

	UPROPERTY(meta = (BindWidget))
	class UImage* CraftingResourceImage;

	UPROPERTY(meta = (BindWidget))
	class UTextBlock* RequiredResourceNum;

	UPROPERTY(meta = (BindWidget))
	class UTextBlock* NeedResourceNum;


	virtual void NativeConstruct() override;

	UFUNCTION(BlueprintCallable, Category = "Crafting")
	void SetListData();

};
