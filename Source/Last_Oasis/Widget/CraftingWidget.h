// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "Components/Button.h"
#include "Components/TextBlock.h"
#include "Components/VerticalBox.h"
#include "Components/CanvasPanel.h"
#include "../Actor/CraftingManager.h"
#include "../Data/RecipeState.h"
#include "../Actor/InventoryManager.h"
#include "CraftingWidget.generated.h"


class UCraftingItemList;
class UCraftingRecipeList;



/**
 * 
 */
UCLASS()
class LAST_OASIS_API UCraftingWidget : public UUserWidget
{
	GENERATED_BODY()
	
public:
	virtual void NativeConstruct() override;

	UPROPERTY(meta = (BindWidget))
	class UButton* CraftingButton;

	UPROPERTY(meta = (BindWidget))
	class UVerticalBox* CraftingRecipeList;

	UPROPERTY(meta = (BindWidget))
	class UVerticalBox* CraftingItemList;

	UPROPERTY(meta = (BindWidget))
	class UCanvasPanel* CraftingUIBox;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Crafting")
	TSubclassOf<UCraftingRecipeList> CraftRecipeClass;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Crafting")
	TSubclassOf<UCraftingItemList> CraftingItemClass;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Crafting")
	ACraftingManager* CraftingManager;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Inventory")
	AInventoryManager* InventoryManager;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Crafting")
	FRecipeState SelectedRecipeState;

	UPROPERTY()
	TArray<UCraftingRecipeList*> RecipeListArray;

	

	UFUNCTION(BlueprintCallable, Category = "Crafting")
	void CraftingRecipeInitialize();

	UFUNCTION(BlueprintCallable, Category = "Crafting")
	void UpdateSelection(UCraftingRecipeList* SelectedRecipe);

	UFUNCTION(BlueprintCallable, Category = "Crafting")
	void CraftingRecipeUpdate();

	UFUNCTION(BlueprintCallable, Category = "Crafting")
	void CraftingItemUpdate();

	UFUNCTION(BlueprintCallable, Category = "Crafting")
	void CraftingItem();
	
	void CloseCraftingUI(int32 index);

};
