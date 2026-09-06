// Fill out your copyright notice in the Description page of Project Settings.

#include "../Widget/ItemInfoWidget.h"
#include "../Data/DataAssetBase.h"

void UItemInfoWidget::SetItemData(UDataAssetBase* ItemData) const
{
	if (!ItemData) return; 

	ItemIcon->SetBrushFromTexture(ItemData->Icon);
	ItemName->SetText(FText::FromName(ItemData->ItemName)); 
	ItemDescription->SetText(ItemData->Description);

	if(ItemData->IsUsable)
		ItemType->SetText(FText::FromString("Usable Item"));
	else
		ItemType->SetText(FText::FromString("Craft Item"));
}
