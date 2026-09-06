// Fill out your copyright notice in the Description page of Project Settings.

#include "../Widget/InventoryWidget.h"
#include "../Data/DataAssetBase.h"
#include "../Data/ItemListDataAsset.h"
#include "../Widget/ItemInfoWidget.h"
#include "../Widget/InventorySlotWidget.h"
#include "Kismet/GameplayStatics.h"

void UInventoryWidget::NativeConstruct()
{
    Super::NativeConstruct();

    InventoryManager = Cast<AInventoryManager>(
        UGameplayStatics::GetActorOfClass(GetWorld(), AInventoryManager::StaticClass()));

    InitializeSlots();
    //UpdateSlot();
}

void UInventoryWidget::InitializeSlots()
{
    if (!GridBox || !ItemSlotWidgetClass || !InventoryManager)
    {
        FTimerHandle RetryHandle;
        GetWorld()->GetTimerManager().SetTimer(RetryHandle, [this]()
            {
                InitializeSlots();
            }, 0.05f, false);
        return;
    }

    const int32 Rows = 6;
    const int32 Columns = 4;

    InventorySlots.Empty();

    for (int32 Row = 0; Row < Rows; ++Row)
    {
        for (int32 Col = 0; Col < Columns; ++Col) 
        {
            UInventorySlotWidget* NewSlot = CreateWidget<UInventorySlotWidget>(GetWorld(), ItemSlotWidgetClass);
            if (!NewSlot) continue;

            NewSlot->ParentInventoryWidget = this;

            UGridSlot* GridSlot = GridBox->AddChildToGrid(NewSlot);
            if (GridSlot)
            {
                GridSlot->SetRow(Row);
                GridSlot->SetColumn(Col);
            }

            InventorySlots.Add(NewSlot);

            NewSlot->ItemImage->SetVisibility(ESlateVisibility::Hidden);
            NewSlot->ItemCount->SetVisibility(ESlateVisibility::Hidden);
            NewSlot->OnSlotClicked.AddDynamic(this, &UInventoryWidget::HandleSlotClicked);
            UE_LOG(LogTemp, Warning, TEXT("Slot binding~~~"));
        }
    }

    if(InventoryManager)
	    InventoryManager->OnInventoryUpdated.AddDynamic(this, &UInventoryWidget::UpdateSlot);

    if (ItemInfoWidget == nullptr || !IsValid(ItemInfoWidget))
    {
        ItemInfoWidget = CreateWidget<UItemInfoWidget>(GetWorld(), ItemInfoWidgetClass);

        if (ItemInfoWidget)
        {
            ItemInfoWidget->AddToViewport(100);
            ItemInfoWidget->SetVisibility(ESlateVisibility::Hidden);
        }       
    }

    UpdateSlot();
}

void UInventoryWidget::HandleSlotClicked(UInventorySlotWidget* ClickedSlot)
{
	UE_LOG(LogTemp, Warning, TEXT("Slot Clicked~~~"));
    UE_LOG(LogTemp, Warning, TEXT("Slot Clicked: %s"),
        ClickedSlot->ItemData ? *ClickedSlot->ItemData->GetName() : TEXT("No ItemData"));
    if (InventoryManager)
    {
        InventoryManager->UseItem(ClickedSlot->ItemData);
    }
    UpdateSlot();
}

void UInventoryWidget::UpdateSlot()
{
    if (!InventoryManager || InventorySlots.Num() == 0) return;

    const int32 SlotCount = InventorySlots.Num();
    const int32 ItemCount = InventoryManager->ItemDataList.Num();

    TArray<FInventoryItem> UsableItems;
    TArray<FInventoryItem> NonUsableItems;

    // ������ �з�
    for (int32 i = 0; i < ItemCount; ++i)
    {
        if (!InventoryManager->ItemDataList[i].ItemData) continue;

        if (InventoryManager->ItemDataList[i].ItemData->IsUsable)
            UsableItems.Add(InventoryManager->ItemDataList[i]);
        else
            NonUsableItems.Add(InventoryManager->ItemDataList[i]);
    }

    // ���ĵ� �迭 ����
    TArray<FInventoryItem> SortedItems;
    SortedItems.Append(UsableItems);
    SortedItems.Append(NonUsableItems);

    // ���Կ� ����
    for (int32 i = 0; i < SlotCount; ++i)
    {
        UInventorySlotWidget* InventorySlot = InventorySlots[i];
        if (!InventorySlot) continue;

        if (i < SortedItems.Num() && SortedItems[i].ItemData)
        {
            InventorySlot->ItemData = SortedItems[i].ItemData;
            InventorySlot->CurrentCount = SortedItems[i].CurrentCount;
        }
        else
        {
            InventorySlot->ItemData = nullptr;
            InventorySlot->CurrentCount = 0;
        }

        InventorySlot->SetSlotData();
    }


}

void UInventoryWidget::UseItem()
{
}

void UInventoryWidget::GetItem(UDataAssetBase* ItemData)
{

}    

