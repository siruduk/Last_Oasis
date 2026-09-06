// Fill out your copyright notice in the Description page of Project Settings.


#include "../Actor/MyActor.h"
#include "../Widget/InventoryWidget.h"

// Sets default values
AMyActor::AMyActor()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

}

// Called when the game starts or when spawned
void AMyActor::BeginPlay()
{
	Super::BeginPlay();

    //if (InventoryWidgetClass)
    //{
    //    UInventoryWidget* InventoryUI = CreateWidget<UInventoryWidget>(GetWorld(), InventoryWidgetClass);
    //    if (InventoryUI)
    //    {
    //        InventoryUI->InventoryActor = this; // 안전하게 자기 자신 전달
    //        InventoryUI->AddToViewport();
    //    }
    //}
}

// Called every frame
void AMyActor::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

