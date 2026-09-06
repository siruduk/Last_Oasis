// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "../Data/DataAssetBase.h"

#include "MyActor.generated.h"

class UInventoryWidget;

UCLASS()
class LAST_OASIS_API AMyActor : public AActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	AMyActor();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Inventory")
	TSubclassOf<UInventoryWidget> InventoryWidgetClass;


	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Inventory")
	TArray<UDataAssetBase*> itemList;
};
