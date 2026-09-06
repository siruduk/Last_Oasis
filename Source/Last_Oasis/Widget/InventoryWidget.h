// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Components/GridPanel.h"
#include "Components/GridSlot.h"
#include "Components/Button.h"
#include "Blueprint/UserWidget.h"
#include "../Data/InventoryItemStruct.h"
#include "../Actor/InventoryManager.h"
#include "InventoryWidget.generated.h"

class UItemInfoWidget;
class UInventorySlotWidget;
class UDataAssetBase;
class UItemListDataAsset;



UCLASS()
class LAST_OASIS_API UInventoryWidget : public UUserWidget
{
	GENERATED_BODY()
	
public:
    // 아이템 정보 위젯
    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Inventory")
    UItemInfoWidget* ItemInfoWidget;

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Inventory")
    TSubclassOf<UItemInfoWidget> ItemInfoWidgetClass;

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Inventory")
    TSubclassOf<UInventorySlotWidget> ItemSlotWidgetClass;

    UPROPERTY()
    TArray<UInventorySlotWidget*> InventorySlots;

    UPROPERTY(meta = (BindWidget))
    UGridPanel* GridBox;

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Inventory")
	AInventoryManager* InventoryManager;

    UFUNCTION()
	void HandleSlotClicked(UInventorySlotWidget* ClickedSlot);

    // ===============Test=============



    // =================================
    UFUNCTION(BlueprintCallable)
    void UseItem();
    UFUNCTION(BlueprintCallable)
    void GetItem(UDataAssetBase* ItemData);

    // =================================


    virtual void NativeConstruct() override;

    UFUNCTION(BlueprintCallable)
    void InitializeSlots();

    UFUNCTION(BlueprintCallable)
    void UpdateSlot();


};
