// Fill out your copyright notice in the Description page of Project Settings.


#include "Player/LOPlayerController.h"
#include "UI/InGameHUD.h"

void ALOPlayerController::InitHUD()
{
	if (HUDWidgetClass)
	{
		HUD = CreateWidget<UInGameHUD>(this, HUDWidgetClass);
		if (HUD)
		{
			HUD->AddToViewport();
			fucking();

			OnHUDInitialized.Broadcast();
		}
	}
}

void ALOPlayerController::fucking()
{
	if (HUD)
	{
		HUD->SetAbilitySystemComponent();
	}
}

void ALOPlayerController::OnPossess(APawn* InPawn)
{
	Super::OnPossess(InPawn);
	if (fuck)
		fucking();
}

void ALOPlayerController::BeginPlay()
{
	Super::BeginPlay();
	
	InitHUD();
	fuck = true;
}