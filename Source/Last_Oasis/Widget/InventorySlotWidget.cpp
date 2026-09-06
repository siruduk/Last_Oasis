// Fill out your copyright notice in the Description page of Project Settings.

#include "../Widget/InventorySlotWidget.h"
#include "../Data/DataAssetBase.h"
#include "Blueprint/WidgetLayoutLibrary.h"
#include "../Widget/ItemInfoWidget.h"
#include "../Widget/InventoryWidget.h"

void UInventorySlotWidget::NativeConstruct()
{
	if (!ParentInventoryWidget->ItemInfoWidget)
		return;

	InfoWidget = ParentInventoryWidget->ItemInfoWidget;
}

FReply UInventorySlotWidget::NativeOnMouseMove(const FGeometry& InGeometry, const FPointerEvent& InMouseEvent)
{

	if (InfoWidget && InfoWidget->GetVisibility() == ESlateVisibility::Visible)
	{
		FVector2D MousePos = InMouseEvent.GetScreenSpacePosition();
		MousePos.Y -= 150.0f;
		MousePos.X -= 420.0f;
		InfoWidget->SetPositionInViewport(MousePos, true);
	}

	return Super::NativeOnMouseMove(InGeometry, InMouseEvent);
}

void UInventorySlotWidget::NativeOnMouseEnter(const FGeometry& InGeometry, const FPointerEvent& InMouseEvent)
{
	Super::NativeOnMouseEnter(InGeometry, InMouseEvent);

	if (!ItemImage || ItemImage->GetVisibility() == ESlateVisibility::Hidden)
		return;

	if (!ParentInventoryWidget || !ParentInventoryWidget->ItemInfoWidget)
		return;

	if (LastHoveredSlot == this) return;

	InfoWidget = ParentInventoryWidget->ItemInfoWidget;

	if (!InfoWidget->IsInViewport())
	{
		InfoWidget->AddToViewport(9999);
	}

	if (InfoWidget->GetVisibility() != ESlateVisibility::Visible)
	{
		InfoWidget->SetVisibility(ESlateVisibility::Visible);
		InfoWidget->SetItemData(ItemData);
	}
	LastHoveredSlot = this;
}

void UInventorySlotWidget::NativeOnMouseLeave(const FPointerEvent& InMouseEvent)
{
	Super::NativeOnMouseLeave(InMouseEvent);

	if (LastHoveredSlot != this) return;

	if (InfoWidget && InfoWidget->IsInViewport())
		InfoWidget->SetVisibility(ESlateVisibility::Hidden);

	LastHoveredSlot = nullptr;
}

FReply UInventorySlotWidget::NativeOnMouseButtonDown(const FGeometry& InGeometry, const FPointerEvent& InMouseEvent)
{

	if (InMouseEvent.GetEffectingButton() == EKeys::LeftMouseButton &&
		ItemData->IsUsable)
	{
		OnSlotClicked.Broadcast(this);
		SetSlotData();
		return FReply::Handled();
	}


	return Super::NativeOnMouseButtonDown(InGeometry, InMouseEvent);
}

void UInventorySlotWidget::SetSlotData()
{

	if (!ItemData || CurrentCount <= 0)
	{
		ItemImage->SetVisibility(ESlateVisibility::Hidden);
		ItemCount->SetVisibility(ESlateVisibility::Hidden);
		return;
	}

	// ���� ���� ����
	if (CurrentCount > ItemData->MaxStackCount)
		CurrentCount = ItemData->MaxStackCount;

	// UI ������Ʈ
	ItemImage->SetBrushFromTexture(ItemData->Icon);
	ItemImage->SetVisibility(ESlateVisibility::Visible);

	if (ItemData->IsStackable)
	{
		ItemCount->SetText(FText::AsNumber(CurrentCount));
		ItemCount->SetVisibility(ESlateVisibility::Visible);
	}
	else
	{
		ItemCount->SetText(FText::AsNumber(1));
		ItemCount->SetVisibility(ESlateVisibility::Hidden);
	}

}

void UInventorySlotWidget::SetOptionVisible()
{
	//if (CurrentCount == 0 || !ItemData)
	if (CurrentCount == 0)
	{
		ItemImage->SetVisibility(ESlateVisibility::Hidden);
		ItemCount->SetVisibility(ESlateVisibility::Hidden);
		return;
	}
	else
	{
		ItemImage->SetVisibility(ESlateVisibility::Visible);
		ItemCount->SetVisibility(ESlateVisibility::Visible);
	}
}
