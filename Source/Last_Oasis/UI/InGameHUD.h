// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "AbilitySystemInterface.h"
#include "GamePlayAbility/AttributeSet/LOAttributeSet.h"
#include "Components/Border.h"
#include "Components/Image.h"
#include "Components/TextBlock.h"
#include "Widget/InventoryWidget.h"
#include "Widget/CraftingWidget.h"
#include "Character/PlayerCharacter.h"
#include "GameFramework/SpringArmComponent.h"
#include "Camera/CameraComponent.h"
#include "InGameHUD.generated.h"

UCLASS()
class LAST_OASIS_API UInGameHUD : public UUserWidget, public IAbilitySystemInterface
{
	GENERATED_BODY()

public:
	void SetAbilitySystemComponent();
	virtual UAbilitySystemComponent* GetAbilitySystemComponent() const override;

	UFUNCTION(BlueprintImplementableEvent, BlueprintCallable)
	void ShowToastMsg(const FString& InText);

	UFUNCTION(BlueprintImplementableEvent, BlueprintCallable)
	void Fade(float Speed = 1);
	UPROPERTY(EditAnywhere)
	TSubclassOf<AActor> TentClass;
	UPROPERTY(EditAnywhere)
	TSubclassOf<AActor> FireClass;
protected:
	virtual void NativeConstruct() override;
	virtual void NativeTick(const FGeometry& MyGeometry, float InDeltaTime) override;

	void OnHealthChanged(const FOnAttributeChangeData& ChangeData);
	void OnThirstChanged(const FOnAttributeChangeData& ChangeData);
	void OnHungerChanged(const FOnAttributeChangeData& ChangeData);	
	void OnTemperatureChanged(const FOnAttributeChangeData& ChangeData);
	void OnSpeedChanged(const FOnAttributeChangeData& ChangeData);
	
	UPROPERTY()
	TObjectPtr<class UCharacterMovementComponent> CharacterMovementComponent;
protected:
	UPROPERTY(EditAnywhere, Category = GAS)
	TObjectPtr<class UAbilitySystemComponent> ASC;

	UPROPERTY(EditAnywhere,Meta=(AllowPrivateAccess = true, BindWidget))
	TObjectPtr<class UProgressBar> Health;

	UPROPERTY(EditAnywhere, Meta = (AllowPrivateAccess = true, BindWidget))
	UImage* Thirst;

	UPROPERTY(EditAnywhere, Meta = (AllowPrivateAccess = true, BindWidget))
	UImage* Hunger;

	UPROPERTY(EditAnywhere, Meta = (AllowPrivateAccess = true, BindWidget))
	UImage* Temperature;

	UPROPERTY(EditAnywhere, Meta = (AllowPrivateAccess = true, BindWidget))
	TObjectPtr<class UTextBlock> ThirstText;

	UPROPERTY(EditAnywhere, Meta = (AllowPrivateAccess = true, BindWidget))
	TObjectPtr<class UTextBlock> HungerText;

	UPROPERTY(EditAnywhere, Meta = (AllowPrivateAccess = true, BindWidget))
	TObjectPtr<class UTextBlock> TemperatureText;

	UPROPERTY()
	UMaterialInstanceDynamic* ThirstMID;

	UPROPERTY()
	UMaterialInstanceDynamic* HungerMID;

	UPROPERTY()
	UMaterialInstanceDynamic* TemperatureMID;


	UPROPERTY(EditAnywhere, Meta = (AllowPrivateAccess = true, BindWidget))
	TObjectPtr<class UTextBlock> DaysText;

	UPROPERTY(EditAnywhere, Meta = (AllowPrivateAccess = true, BindWidget))
	TObjectPtr<class UImage> Day_Night;

	UPROPERTY(EditAnywhere, Meta = (AllowPrivateAccess = true, BindWidget))
	TObjectPtr<class UTextBlock> TimeText;

	UPROPERTY(EditAnywhere, Meta = (AllowPrivateAccess = true, BindWidget))
	TObjectPtr<class UImage> Raider;

	UPROPERTY(EditAnywhere, Meta = (AllowPrivateAccess = true, BindWidget))
	TObjectPtr<class UImage> LaboA;

	UPROPERTY(EditAnywhere, Meta = (AllowPrivateAccess = true, BindWidget))
	TObjectPtr<class UImage> LaboB;

	UPROPERTY(EditAnywhere, Meta = (AllowPrivateAccess = true, BindWidget))
	TObjectPtr<class UImage> Oasis;

	UPROPERTY(EditAnywhere, Meta = (AllowPrivateAccess = true, BindWidget))
	TObjectPtr<class UBorder> Goal;

	UPROPERTY(EditAnywhere, Meta = (AllowPrivateAccess = true, BindWidget))
	TObjectPtr<class UTextBlock> GoalText;

	UPROPERTY(EditAnywhere, Meta = (AllowPrivateAccess = true, BindWidget))
	TObjectPtr<class UTextBlock> GoalBarText;

	UPROPERTY(EditAnywhere, Meta = (AllowPrivateAccess = true, BindWidget))
	TObjectPtr<class UImage> GoalArrow;

	UPROPERTY(EditAnywhere, Meta = (AllowPrivateAccess = true, BindWidget))
	TObjectPtr<class UInventoryWidget> InventoryWidget;

	UPROPERTY(EditAnywhere, Meta = (AllowPrivateAccess = true, BindWidget))
	TObjectPtr<class UCraftingWidget> CraftingWidget;

	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite)
	TSubclassOf<class UCraftingWidget> CraftingWidgetClass;

	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite)
	TSubclassOf<class UInventoryWidget> InventoryWidgetClass;


	UTexture2D* SunTexture;
	UTexture2D* MoonTexture;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	TArray<FString> GoalTexts;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	TArray<FString> GoalBarTexts;

	APlayerCharacter* Player;
	UCameraComponent* PlayerCamera;
	USpringArmComponent* PlayerSpringArm;

	FVector LaboALoc;
	FVector LaboBLoc;
	FVector OasisLoc;

	int32 CurrentPhase = 0;

public:
	UFUNCTION(BlueprintCallable, Category = "UI")
	UInventoryWidget* GetInventoryWidget() const { return InventoryWidget; }

	UFUNCTION(BlueprintCallable, Category = "UI")
	UCraftingWidget* GetCraftingWidget() const { return CraftingWidget; }

public:
	UFUNCTION()
	void UpdateTime(int32 Hour, int32 Minute);

	UFUNCTION()
	void UpdateDays(int32 Days);

	UFUNCTION()
	void InitProgress(UImage*& State, UMaterialInstanceDynamic*& MID);

	UFUNCTION()
    void UpdateProgress(UMaterialInstanceDynamic*& MID, float Percent);

	UFUNCTION()
	void OpenGoal();

	UFUNCTION()
	void SetGoalText(int32 Phase);

	FTimerHandle BorderMoveTimerHandle;
	float ElapsedTime;
	float TotalDuration;
	FVector2D StartPos;
	FVector2D EndPos;

	UPROPERTY()
	bool bIsBorderMoving = false;

	void UpdateBorderPosition();

	void SetBuildings(APlayerCharacter* Ch, FVector LaboAPos, FVector LaboBPos, FVector OasisPos);

	float GetAngleToTarget(const FVector& TargetLoc);

	void ApplyMarkerRotation(UImage* Marker, float Angle);


	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite)
	TObjectPtr<class UWidget> TempWidget;
};
