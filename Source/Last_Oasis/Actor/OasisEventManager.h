// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "OasisEventManager.generated.h"

class ACraftingManager;

UCLASS()
class LAST_OASIS_API AOasisEventManager : public AActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	AOasisEventManager();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

public:
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Crafting")
	TSubclassOf<AActor> StoryLightActorClass;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Crafting")
	TArray<FVector> StoryLightPositions;


	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Crafting")
	ACraftingManager* CraftingManager;

	UFUNCTION(BlueprintCallable, Category = "Oasis Event")
	void LightEvent(int32 index);

	UFUNCTION(CallInEditor,BlueprintCallable, Category = "Oasis Event")
	void CameraCutsceneAtLocation(FVector TargetLocation, float Duration = 3.f, float ZoomFOV = 60.f);

	FVector CustomFocusLocation;
	bool bUseCustomFocus = false;

	UFUNCTION(CallInEditor,BlueprintCallable, Category = "Oasis Event")
	void TestLightEvent();

public:
	UPROPERTY()
	APlayerController* FocusPC;

	UPROPERTY()
	AActor* FocusTarget;


	FRotator OriginalRotation;
	float OriginalFOV;
	float TargetFOV;

	bool bIsCameraInCutscene = false;
	bool bReturning = false; // 원래 상태로 돌아오는 단계
	float CameraInterpSpeed = 2.0f;
	float FOVInterpSpeed = 2.0f;


};
