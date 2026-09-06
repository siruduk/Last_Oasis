// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "Components/TextBlock.h"
#include "Components/Image.h"
#include "InventorySlotWidget.generated.h"

class UDataAssetBase;
class UItemInfoWidget;
class UInventoryWidget;

DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FOnSlotClicked, UInventorySlotWidget*, ClickedSlot);


/**
 * 
 */
UCLASS()
class LAST_OASIS_API UInventorySlotWidget : public UUserWidget
{
	GENERATED_BODY()
public:

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Inventory")
    class UInventoryWidget* ParentInventoryWidget;

    // 슬롯에 들어갈 아이템 정보
    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Inventory")
    UDataAssetBase* ItemData;

    UItemInfoWidget* InfoWidget;

    // 현재 소지 개수
    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Inventory")
    int32 CurrentCount = 0;

    UPROPERTY(meta = (BindWidget))
    UImage* BackgroundImage;

    UPROPERTY(meta = (BindWidget))
    UImage* ItemImage;

    UPROPERTY(meta = (BindWidget))
    UTextBlock* ItemCount;

    virtual void NativeConstruct() override;
    virtual FReply NativeOnMouseMove(const FGeometry& InGeometry, const FPointerEvent& InMouseEvent) override;
    virtual void NativeOnMouseEnter(const FGeometry& InGeometry, const FPointerEvent& InMouseEvent) override;
    virtual void NativeOnMouseLeave(const FPointerEvent& InMouseEvent) override;

    UPROPERTY(BlueprintAssignable, Category = "Inventory")
    FOnSlotClicked OnSlotClicked;

  virtual FReply NativeOnMouseButtonDown( const FGeometry& InGeometry, const FPointerEvent& InMouseEvent) override;

    UFUNCTION(BlueprintCallable, Category = "Inventory")
    void SetSlotData();

    void SetOptionVisible();

    UPROPERTY()
    UInventorySlotWidget* LastHoveredSlot = nullptr;

};
