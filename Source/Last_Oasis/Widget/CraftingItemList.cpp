// Fill out your copyright notice in the Description page of Project Settings.


#include "../Widget/CraftingItemList.h"
#include "../Widget/CraftingWidget.h"
#include "../Actor/CraftingManager.h"


void UCraftingItemList::NativeConstruct()
{
}

void UCraftingItemList::SetListData()
{
	CraftingResourceName->SetText(FText::FromName(ResourceData.Resource->ItemName));
	RequiredResourceNum->SetText(FText::AsNumber(ResourceData.NeedCount));
	NeedResourceNum->SetText(FText::AsNumber(InventoryItemCount));
	CraftingResourceImage->SetBrushFromTexture(ResourceData.Resource->Icon);

	if (ResourceData.NeedCount <= InventoryItemCount)
		bIsSufficientResource = true;
	else
		bIsSufficientResource = false;
}
