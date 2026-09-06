// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/PlayerController.h"
#include "LOPlayerController.generated.h"

DECLARE_DYNAMIC_MULTICAST_DELEGATE(FOnHUDInitialized);

UCLASS()
class LAST_OASIS_API ALOPlayerController : public APlayerController
{
	GENERATED_BODY()

public:
	void InitHUD();
	void fucking();
	virtual void OnPossess(APawn* InPawn) override;
	virtual void BeginPlay() override;

	UPROPERTY(EditAnywhere)
	TSubclassOf<class UInGameHUD> HUDWidgetClass;
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	TObjectPtr<class UInGameHUD> HUD;

	UPROPERTY(BlueprintAssignable)
	FOnHUDInitialized OnHUDInitialized;
	bool fuck = false;

};
