// Fill out your copyright notice in the Description page of Project Settings.


#include "Actor/OasisEventManager.h"

#include "../Actor/CraftingManager.h"
#include "../Character/PlayerCharacter.h"
#include "Kismet/GameplayStatics.h"
#include "GameMode/LOGameModeBase.h"
#include "UI/InGameHUD.h"

// Sets default values
AOasisEventManager::AOasisEventManager()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

}

// Called when the game starts or when spawned
void AOasisEventManager::BeginPlay()
{
	Super::BeginPlay();
	
	AGameModeBase* GameMode = UGameplayStatics::GetGameMode(GetWorld());
	ALOGameModeBase* LOGameMode = Cast<ALOGameModeBase>(GameMode);

	for (int i = 0; i < LOGameMode->SpawnedBuildings.Num(); i++)
	{
		StoryLightPositions[i] = LOGameMode->SpawnedBuildings[i]->GetActorLocation();
	}

	CraftingManager= Cast<ACraftingManager>(
		UGameplayStatics::GetActorOfClass(GetWorld(), ACraftingManager::StaticClass()));
	if (CraftingManager)
	{
		CraftingManager->OnCraftingEvent.AddDynamic(this, &AOasisEventManager::LightEvent);
	}

}

void AOasisEventManager::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	if (!bIsCameraInCutscene || !FocusPC) return;

	FVector CameraLocation = FocusPC->PlayerCameraManager->GetCameraLocation();
	FVector TargetLocation;

	if (bReturning)
		TargetLocation = CameraLocation + FocusPC->GetControlRotation().Vector();
	else if (bUseCustomFocus)
		TargetLocation = CustomFocusLocation;
	else if (FocusTarget)
		TargetLocation = FocusTarget->GetActorLocation();
	else
		return;

	FRotator CurrentRotation = FocusPC->GetControlRotation();
	FRotator DesiredRotation = bReturning ? OriginalRotation : (TargetLocation - CameraLocation).Rotation();
	FRotator NewRotation = FMath::RInterpTo(CurrentRotation, DesiredRotation, DeltaTime, CameraInterpSpeed);
	FocusPC->SetControlRotation(NewRotation);

	if (FocusPC->PlayerCameraManager)
	{
		float CurrentFOV = FocusPC->PlayerCameraManager->GetFOVAngle();
		float NewFOV = FMath::FInterpTo(CurrentFOV, TargetFOV, DeltaTime, FOVInterpSpeed);
		FocusPC->PlayerCameraManager->SetFOV(NewFOV);
	}

	if (bReturning)
	{
		bool bRotationClose = FocusPC->GetControlRotation().Equals(OriginalRotation, 0.1f);
		bool bFOVClose = FMath::IsNearlyEqual(FocusPC->PlayerCameraManager->GetFOVAngle(), OriginalFOV, 0.1f);

		if (bRotationClose && bFOVClose)
		{
			bIsCameraInCutscene = false;
			bReturning = false;
			bUseCustomFocus = false;

			FocusPC->SetIgnoreMoveInput(false);
			FocusPC->SetIgnoreLookInput(false);
		}
	}
}

void AOasisEventManager::LightEvent(int32 index)
{
	UE_LOG(LogTemp, Warning, TEXT("Light Event Index : %d"), index);

	if (index == 0)
	{
		// Phase 1
		ALOPlayerController* PlayerContr = Cast<ALOPlayerController>(GetWorld()->GetFirstPlayerController());
		PlayerContr->HUD->SetGoalText(1);
	}
	else if (index == 3)
	{
		// Phase 1
		ALOPlayerController* PlayerContr = Cast<ALOPlayerController>(GetWorld()->GetFirstPlayerController());
		PlayerContr->HUD->SetGoalText(3);
	}

	FocusPC = GetWorld()->GetFirstPlayerController();
	if (!FocusPC) return;

	APlayerCharacter* Player = Cast<APlayerCharacter>(FocusPC->GetPawn());
	if (!Player) return;

	FocusPC->SetIgnoreMoveInput(true);
	FocusPC->SetIgnoreLookInput(true);

	FActorSpawnParameters SpawnParams;
	FocusTarget = GetWorld()->SpawnActor<AActor>(
		StoryLightActorClass,
		StoryLightPositions[index],
		FRotator::ZeroRotator,
		SpawnParams
	);
	if (!FocusTarget) return;

	OriginalRotation = FocusPC->GetControlRotation();
	OriginalFOV = FocusPC->PlayerCameraManager ? FocusPC->PlayerCameraManager->GetFOVAngle() : 90.f;

	TargetFOV = 60.f;

	bIsCameraInCutscene = true;
	bReturning = false;

	FTimerHandle TimerHandle;
	GetWorld()->GetTimerManager().SetTimer(TimerHandle, [this]()
		{
			bReturning = true;
			TargetFOV = OriginalFOV;
		}, 3.0f, false);

}

void AOasisEventManager::CameraCutsceneAtLocation(FVector TargetLocation, float Duration, float ZoomFOV)
{
	FocusPC = GetWorld()->GetFirstPlayerController();
	if (!FocusPC) return;

	APlayerCharacter* Player = Cast<APlayerCharacter>(FocusPC->GetPawn());
	if (!Player) return;

	FocusPC->SetIgnoreMoveInput(true);
	FocusPC->SetIgnoreLookInput(true);

	CustomFocusLocation = TargetLocation; 
	bUseCustomFocus = true;             

	OriginalRotation = FocusPC->GetControlRotation();
	OriginalFOV = FocusPC->PlayerCameraManager ? FocusPC->PlayerCameraManager->GetFOVAngle() : 90.f;

	TargetFOV = ZoomFOV;

	bIsCameraInCutscene = true;
	bReturning = false;

	FTimerHandle TimerHandle;
	GetWorld()->GetTimerManager().SetTimer(TimerHandle, [this]()
		{
			bReturning = true;
			TargetFOV = OriginalFOV;
		}, Duration, false);

}

void AOasisEventManager::TestLightEvent()
{
	FocusPC = GetWorld()->GetFirstPlayerController();
	if (!FocusPC) return;

	APlayerCharacter* Player = Cast<APlayerCharacter>(FocusPC->GetPawn());
	if (!Player) return;

	FocusPC->SetIgnoreMoveInput(true);
	FocusPC->SetIgnoreLookInput(true);

	FActorSpawnParameters SpawnParams;
	FocusTarget = GetWorld()->SpawnActor<AActor>(
		StoryLightActorClass,
		StoryLightPositions[0],
		FRotator::ZeroRotator,
		SpawnParams
	);
	if (!FocusTarget) return;

	OriginalRotation = FocusPC->GetControlRotation();
	OriginalFOV = FocusPC->PlayerCameraManager ? FocusPC->PlayerCameraManager->GetFOVAngle() : 90.f;

	TargetFOV = 60.f;

	bIsCameraInCutscene = true;
	bReturning = false;

	FTimerHandle TimerHandle;
	GetWorld()->GetTimerManager().SetTimer(TimerHandle, [this]()
		{
			bReturning = true;
			TargetFOV = OriginalFOV; 
		}, 3.0f, false); 
}
