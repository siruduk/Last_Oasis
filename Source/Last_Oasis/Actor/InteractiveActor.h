// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "InteractiveActor.generated.h"

class UDataAssetBase;;

USTRUCT(BlueprintType)
struct FDropItemData
{
	GENERATED_BODY()

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	UDataAssetBase* DropItemData;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	float DropChance;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	int32 DropMinRange;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	int32 DropMaxRange; 
};



UCLASS()
class LAST_OASIS_API AInteractiveActor : public AActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	AInteractiveActor();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Data")
	TArray<FDropItemData> DropItems;

};
