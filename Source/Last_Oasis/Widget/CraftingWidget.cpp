// Fill out your copyright notice in the Description page of Project Settings.


#include "../Widget/CraftingWidget.h"
#include "../Widget/CraftingItemList.h"
#include "../Widget/CraftingRecipeList.h"
#include "Kismet/GameplayStatics.h"
#include "Data/DataAssetBase.h"


void UCraftingWidget::NativeConstruct()
{
	Super::NativeConstruct();

    if (!GetWorld() || !GetWorld()->IsGameWorld())
        return;
	CraftingManager = Cast<ACraftingManager>(
		UGameplayStatics::GetActorOfClass(GetWorld(), ACraftingManager::StaticClass()));

    InventoryManager = Cast<AInventoryManager>(
        UGameplayStatics::GetActorOfClass(GetWorld(), AInventoryManager::StaticClass()));

	CraftingUIBox->SetVisibility(ESlateVisibility::Hidden);

    if (InventoryManager)
    {
        InventoryManager->OnRecipeUpdated.AddDynamic(this, &UCraftingWidget::CraftingRecipeUpdate);
    }

	CraftingRecipeInitialize();

}

void UCraftingWidget::CraftingRecipeInitialize()
{
    if (!IsValid(CraftingManager))
    {
        FTimerHandle RetryHandle;
        GetWorld()->GetTimerManager().SetTimer(
            RetryHandle, this, &UCraftingWidget::CraftingRecipeInitialize, 
            0.05f, false
        );

        return;
    }

    CraftingRecipeList->ClearChildren(); 
    RecipeListArray.Empty();

    const int32 RecipeCount = CraftingManager->RecipeStates.Num();

    // 해금
    for (int32 i = 0; i < RecipeCount; ++i) //41
    {
        if (CraftingManager->RecipeStates[i].bUnlocked) // bUnlocked : true
        {
            UCraftingRecipeList* NewRecipeWidget = CreateWidget<UCraftingRecipeList>(GetWorld(), CraftRecipeClass);
            if (NewRecipeWidget)
            {
                NewRecipeWidget->ParentCraftingWidget = this;
                NewRecipeWidget->ItemData = CraftingManager->RecipeStates[i].RecipeItem;
                NewRecipeWidget->SetRecipeData();

                CraftingRecipeList->AddChild(NewRecipeWidget);
                RecipeListArray.Add(NewRecipeWidget);
            }
        }
    }

    // 미해금
    for (int32 i = 0; i < RecipeCount; ++i)
    {
        if (!CraftingManager->RecipeStates[i].bUnlocked) // bUnlocked : false
        {
            UCraftingRecipeList* NewRecipeWidget = CreateWidget<UCraftingRecipeList>(GetWorld(), CraftRecipeClass);
            if (NewRecipeWidget)
            {
                // 데이터 전달
                CraftingRecipeList->AddChild(NewRecipeWidget);
                NewRecipeWidget->ParentCraftingWidget = this;
                NewRecipeWidget->SetVisibility(ESlateVisibility::Hidden);
                RecipeListArray.Add(NewRecipeWidget);
            }
        }
    }
}

void UCraftingWidget::UpdateSelection(UCraftingRecipeList* SelectedRecipe)
{
	if (!SelectedRecipe) return;

    for (UCraftingRecipeList* Item : RecipeListArray)
    {
        if (Item != SelectedRecipe && Item->bIsSelected)
        {
            // 다른 아이템 선택 해제
            Item->SetSelected(false);
            Item->bIsSelected = false;
        }
    }

    bool bSameRecipee = (SelectedRecipeState.RecipeItem == SelectedRecipe->ItemData);

    SelectedRecipe->bIsSelected = true;
    SelectedRecipe->SetSelected(true);

    for (const FRecipeState& Recipe : CraftingManager->RecipeStates)
    {
        if (Recipe.RecipeItem == SelectedRecipe->ItemData)
        {
            SelectedRecipeState = Recipe; // 선택된 레시피 전체 저장
            break;
        }
    }

    if (CraftingUIBox->GetVisibility() != ESlateVisibility::Visible)
        CraftingUIBox->SetVisibility(ESlateVisibility::Visible);

    // 선택된 Recipe의 ItemList 업데이트
    CraftingItemUpdate();
}

// 켜고 끌때마다 업데이트
void UCraftingWidget::CraftingRecipeUpdate()
{
    if (!CraftingManager) return;

    TArray<UUserWidget*> UnlockedWidgets;
    TArray<UUserWidget*> LockedWidgets;

    // 기존 위젯 분류
    for (int32 i = 0; i < RecipeListArray.Num(); ++i)
    {
        UCraftingRecipeList* Widget = RecipeListArray[i];
        if (!Widget) continue;

        const FRecipeState& RecipeState = CraftingManager->RecipeStates[i];

        // 데이터 갱신
        Widget->ItemData = RecipeState.RecipeItem;
        Widget->SetRecipeData();

        // Visible 처리
        if (RecipeState.bUnlocked)
        {
            Widget->SetVisibility(ESlateVisibility::Visible);
            UnlockedWidgets.Add(Widget);
        }
        else
        {
            Widget->SetVisibility(ESlateVisibility::Hidden);
            LockedWidgets.Add(Widget);
        }
    }

    // UI 순서 재배치
    CraftingRecipeList->ClearChildren();

    for (UUserWidget* Widget : UnlockedWidgets)
    {
        CraftingRecipeList->AddChild(Widget);
    }

    for (UUserWidget* Widget : LockedWidgets)
    {
        CraftingRecipeList->AddChild(Widget);
    }

    //CraftingRecipeInitialize();
	// 레시피 업데이트할때, ItemList도 업데이트
    CraftingItemUpdate();
}

void UCraftingWidget::CraftingItemUpdate()
{
    CraftingItemList->ClearChildren();
    bool bAllSufficient = true;

    for (const FRecipeResource& Resource : SelectedRecipeState.RequiredResources)
    {
        UCraftingItemList* NewItemWidget = CreateWidget<UCraftingItemList>(GetWorld(), CraftingItemClass);
        NewItemWidget->ParentCraftingWidget = this;
        NewItemWidget->CraftingManager = CraftingManager;
        
        int32 ResourceCountInInventory = 0;
        for (const FInventoryItem& InventoryItem : InventoryManager->ItemDataList)
        {
            if (InventoryItem.ItemData == Resource.Resource) 
            {
                ResourceCountInInventory = InventoryItem.CurrentCount;
                break; 
            }
        }
		NewItemWidget->InventoryItemCount = ResourceCountInInventory;

        NewItemWidget->ResourceData = Resource;
        NewItemWidget->SetListData();

        if (!NewItemWidget->bIsSufficientResource)
        {
            bAllSufficient = false;
        }

        CraftingItemList->AddChild(NewItemWidget);
    }
    

    if (CraftingButton)
    {
        CraftingButton->SetIsEnabled(bAllSufficient);
    }
}

void UCraftingWidget::CraftingItem()
{
    for (const FRecipeResource& Resource : SelectedRecipeState.RequiredResources)
    {
        for (FInventoryItem& InventoryItem : InventoryManager->ItemDataList)
        {
            if (InventoryItem.ItemData == Resource.Resource)
            {
                InventoryItem.CurrentCount -= Resource.NeedCount;
                break; // 한 번 차감 후 다음 재료
            }
        }
    }

    bool bFound = false;
    for (FInventoryItem& InventoryItem : InventoryManager->ItemDataList)
    {
        if (InventoryItem.ItemData == SelectedRecipeState.RecipeItem)
        {
            InventoryItem.CurrentCount += 1;
            bFound = true;
            break;
        }
    }
    if (!bFound)
    {
        InventoryManager->ItemDataList.Add(FInventoryItem{ SelectedRecipeState.RecipeItem, 1 });
    }

    // 인벤토리 업데이트
    InventoryManager->UpdateBroadCast();




    for (int i = 0; i < CraftingManager->FirstStoryItemData.Num(); i++)
    {
        if (CraftingManager->FirstStoryItemData[i] == SelectedRecipeState.RecipeItem)
        {
            FTimerHandle CloseHandle;
            GetWorld()->GetTimerManager().SetTimer(
                CloseHandle,
                FTimerDelegate::CreateUObject(this, &UCraftingWidget::CloseCraftingUI, 0),
                1.0f, false
            );
			CraftingManager->FirstStoryItemData.Empty();
            break;
        }
    }

    for (int i = 0; i < CraftingManager->SecondStoryItemData.Num(); i++)
    {
        if (CraftingManager->SecondStoryItemData[i] == SelectedRecipeState.RecipeItem)
        {
            FTimerHandle CloseHandle;
            GetWorld()->GetTimerManager().SetTimer(
                CloseHandle,
                FTimerDelegate::CreateUObject(this, &UCraftingWidget::CloseCraftingUI, 1),
                1.0f, false
            );
            CraftingManager->SecondStoryItemData.Empty();
            break;
        }
    }
    
    if (SelectedRecipeState.RecipeItem == CraftingManager->WeapeonCraftingItem)
    {
		CraftingManager->WeaponCrafting();
    }

    if (SelectedRecipeState.RecipeItem == CraftingManager->DustMaskCraftingItem)
    {
		CraftingManager->DuskMaskCrafting();
    }

	// 제작창 업데이트
    CraftingItemUpdate();
}

void UCraftingWidget::CloseCraftingUI(int32 index)
{
    CraftingManager->StoryItemCraftingEvent(index);

    //숨기기
    if (GetVisibility() == ESlateVisibility::Visible)
    {
        SetVisibility(ESlateVisibility::Hidden);
    }
}