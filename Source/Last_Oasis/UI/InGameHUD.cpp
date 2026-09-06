// Fill out your copyright notice in the Description page of Project Settings.


#include "UI/InGameHUD.h"
#include "AbilitySystemBlueprintLibrary.h"
#include "Components/ProgressBar.h"
#include "GameMode/LOGameModeBase.h"
#include "Blueprint/WidgetLayoutLibrary.h"
#include "Character/PlayerCharacter.h"
#include "Kismet/KismetMathLibrary.h"
#include "Components/CanvasPanelSlot.h"
#include "Data/GoalDataAsset.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "Kismet/GameplayStatics.h"

void UInGameHUD::NativeConstruct()
{
	Super::NativeConstruct();

	SunTexture = LoadObject<UTexture2D>(nullptr, TEXT("/Game/Mireu/Data/Image/MagicalGirl_Sun.MagicalGirl_Sun"));
	MoonTexture = LoadObject<UTexture2D>(nullptr, TEXT("/Game/Mireu/Data/Image/MagicalGirl_DarkSun.MagicalGirl_DarkSun"));

	UGoalDataAsset* GoalDA = Cast<UGoalDataAsset>(StaticLoadObject(
		UGoalDataAsset::StaticClass(),
		nullptr,
		TEXT("/Game/_shuby/DataAssets/GoalDA.GoalDA")
	));

	if (GoalDA)
	{
		GoalTexts = GoalDA->GoalTextArray;
		GoalBarTexts = GoalDA->GoalBarTextArray;
	}

	InitProgress(Thirst, ThirstMID);
	InitProgress(Hunger, HungerMID);
	InitProgress(Temperature, TemperatureMID);

	UpdateTime(6, 0);
	UpdateDays(1);

	UpdateProgress(ThirstMID, 0.9f);
	UpdateProgress(HungerMID, 0.9f);
	UpdateProgress(TemperatureMID, 0.5f);

	SetGoalText(0);

	Raider->SetVisibility(ESlateVisibility::Hidden);
	LaboA->SetVisibility(ESlateVisibility::Hidden);
	LaboB->SetVisibility(ESlateVisibility::Hidden);
	Oasis->SetVisibility(ESlateVisibility::Hidden);

	InventoryWidget->SetVisibility(ESlateVisibility::Hidden);
	CraftingWidget->SetVisibility(ESlateVisibility::Hidden);

}

void UInGameHUD::NativeTick(const FGeometry& MyGeometry, float InDeltaTime)
{
	Super::NativeTick(MyGeometry, InDeltaTime);

	if (!Player) return;

	if (CurrentPhase >= 1)
	{
		float AngleA = GetAngleToTarget(LaboALoc);
		ApplyMarkerRotation(LaboA, AngleA);

		float AngleB = GetAngleToTarget(LaboBLoc);
		ApplyMarkerRotation(LaboB, AngleB);
	}

	if (CurrentPhase == 3)
	{
		float AngleC = GetAngleToTarget(OasisLoc);
		ApplyMarkerRotation(Oasis, AngleC);
	}
}

void UInGameHUD::SetAbilitySystemComponent()
{
	ASC = Cast<IAbilitySystemInterface>(GetOwningPlayer()->GetPawn())->GetAbilitySystemComponent();
	if (IsValid(ASC))
	{
		UE_LOG(LogTemp,Log,TEXT("BindingDelegate"));

		ASC->GetGameplayAttributeValueChangeDelegate(ULOAttributeSet::GetHealthAttribute()).AddUObject(this, &UInGameHUD::OnHealthChanged);
		ASC->GetGameplayAttributeValueChangeDelegate(ULOAttributeSet::GetHungerAttribute()).AddUObject(this, &UInGameHUD::OnHungerChanged);
		ASC->GetGameplayAttributeValueChangeDelegate(ULOAttributeSet::GetThirstAttribute()).AddUObject(this, &UInGameHUD::OnThirstChanged);
		ASC->GetGameplayAttributeValueChangeDelegate(ULOAttributeSet::GetTemperatureAttribute()).AddUObject(this, &UInGameHUD::OnTemperatureChanged);
		ASC->GetGameplayAttributeValueChangeDelegate(ULOAttributeSet::GetSpeedAttribute()).AddUObject(this, &UInGameHUD::OnSpeedChanged);
		CharacterMovementComponent = GetWorld()->GetFirstPlayerController()->GetCharacter()->GetCharacterMovement();
	}
	else
	{
		UE_LOG(LogTemp,Log,TEXT("ASCNULL"));

		GetWorld()->GetTimerManager().SetTimerForNextTick(this, &UInGameHUD::SetAbilitySystemComponent);
	}

}

UAbilitySystemComponent* UInGameHUD::GetAbilitySystemComponent() const
{
	return ASC;
}

void UInGameHUD::OnHealthChanged(const FOnAttributeChangeData& ChangeData)
{
	UE_LOG(LogTemp,Log,TEXT("HealthChange"));
	Health->SetPercent(ChangeData.NewValue/100);
}

void UInGameHUD::OnThirstChanged(const FOnAttributeChangeData& ChangeData)
{
	UpdateProgress(ThirstMID, ChangeData.NewValue / 100);
	UE_LOG(LogTemp,Log,TEXT("ThirstChange"));

	FString NewText = FString::Printf(TEXT("%.1f"), ChangeData.NewValue);
	ThirstText->SetText(FText::FromString(NewText));
}

void UInGameHUD::OnHungerChanged(const FOnAttributeChangeData& ChangeData)
{
	UpdateProgress(HungerMID, ChangeData.NewValue/100);
	UE_LOG(LogTemp,Log,TEXT("HungerChange"));

	FString NewText = FString::Printf(TEXT("%.1f"), ChangeData.NewValue);
	HungerText->SetText(FText::FromString(NewText));
}

void UInGameHUD::OnTemperatureChanged(const FOnAttributeChangeData& ChangeData)
{
	UpdateProgress(TemperatureMID, (ChangeData.NewValue - 31.5) / 10);

	FString NewText = FString::Printf(TEXT("%.1f"), ChangeData.NewValue);
	TemperatureText->SetText(FText::FromString(NewText));
}

void UInGameHUD::OnSpeedChanged(const FOnAttributeChangeData& ChangeData)
{
	CharacterMovementComponent->MaxWalkSpeed = ChangeData.NewValue;
}

void UInGameHUD::UpdateTime(int32 Hour, int32 Minute)
{
	if (TimeText)
	{
		FString NewText = FString::Printf(TEXT("%d : %02d"), Hour, Minute);

		TimeText->SetText(FText::FromString(NewText));

		if(Day_Night)
		{
			if(Day_Night && IsValid(ASC))
			{
				int TotalMinutes = Hour * 60 + Minute;

			
				if (TotalMinutes >= 6 * 60 && TotalMinutes < 20 * 60 + 24)
				{
					Day_Night->SetBrushFromTexture(SunTexture);
					if (ASC->HasMatchingGameplayTag(FGameplayTag::RequestGameplayTag("Game.Night")))
					{
						ASC->RemoveLooseGameplayTag(FGameplayTag::RequestGameplayTag("Game.Night"));
						ASC->AddLooseGameplayTag(FGameplayTag::RequestGameplayTag("Game.Day"));
						Day_Night->SetBrushFromTexture(SunTexture);
						TArray<AActor*>Tents;
						UGameplayStatics::GetAllActorsOfClass(GetWorld(), TentClass,Tents);
						for (auto a : Tents)
						{
							a->Destroy();
						}
						UGameplayStatics::GetAllActorsOfClass(GetWorld(), FireClass,Tents);
						for (auto a : Tents)
						{
							a->Destroy();
						}
					}
				}
				else
				{
					Day_Night->SetBrushFromTexture(MoonTexture);
					if (ASC->HasMatchingGameplayTag(FGameplayTag::RequestGameplayTag("Game.Day")))
					{
						ASC->RemoveLooseGameplayTag(FGameplayTag::RequestGameplayTag("Game.Day"));
						ASC->AddLooseGameplayTag(FGameplayTag::RequestGameplayTag("Game.Night"));
						Day_Night->SetBrushFromTexture(MoonTexture);
					}
				}
			}
		}
	}
}

void UInGameHUD::UpdateDays(int32 Days)
{
	if(DaysText)
	{
		if (IsValid(ASC))
		{
			FString NewText = FString::Printf(TEXT("%d Days"), Days);

			DaysText->SetText(FText::FromString(NewText));
			if (Days == 6 || Days == 12)
			{
				ASC->ApplyModToAttribute(ULOAttributeSet::GetLevelAttribute(),EGameplayModOp::Additive,1);
				FGameplayTagContainer Tag;
				Tag.AddTag(FGameplayTag::RequestGameplayTag("State.Reduction"));
				ASC->CancelAbilities(&Tag);
				ASC->TryActivateAbilitiesByTag(Tag);
			}
		}
	}
}

void UInGameHUD::InitProgress(UImage*& State, UMaterialInstanceDynamic*& MID)
{	
	if (State && State->GetBrush().GetResourceObject())
	{
		MID = UMaterialInstanceDynamic::Create(
			Cast<UMaterialInterface>(State->GetBrush().GetResourceObject()),
			this
		);

		State->SetBrushFromMaterial(MID);
	}
}

void UInGameHUD::UpdateProgress(UMaterialInstanceDynamic*& MID, float Percent)
{
	if (MID)
	{
		MID->SetScalarParameterValue(FName("Percent"), Percent);
	}
}

void UInGameHUD::OpenGoal()
{
	if (bIsBorderMoving) return;

	bIsBorderMoving = true;

	ElapsedTime = 0.f;
	TotalDuration = 1.0f;

	StartPos = UWidgetLayoutLibrary::SlotAsCanvasSlot(Goal)->GetPosition();

	if (StartPos.Y != 180 && StartPos.Y != -30)
	{
		bIsBorderMoving = false;
		return;
	}

	EndPos = StartPos;
	EndPos.Y = (StartPos.Y == 180) ? -30 : 180;

	GetWorld()->GetTimerManager().SetTimer(BorderMoveTimerHandle, this, &UInGameHUD::UpdateBorderPosition, 0.01f, true);

	SetGoalText(CurrentPhase + 1);
}

void UInGameHUD::UpdateBorderPosition()
{
	if (!Goal)
	{
		GetWorld()->GetTimerManager().ClearTimer(BorderMoveTimerHandle);
		bIsBorderMoving = false;
		return;
	}

	ElapsedTime += 0.01f;
	float Alpha = FMath::Clamp(ElapsedTime / TotalDuration, 0.f, 1.f);

	FVector2D NewPos = FMath::Lerp(StartPos, EndPos, Alpha);

	if (UCanvasPanelSlot* CanvasSlot = UWidgetLayoutLibrary::SlotAsCanvasSlot(Goal))
	{
		CanvasSlot->SetPosition(NewPos);
	}

	if (Alpha >= 1.f)
	{
		GetWorld()->GetTimerManager().ClearTimer(BorderMoveTimerHandle);
		bIsBorderMoving = false;
	}
}

void UInGameHUD::SetGoalText(int32 Phase)
{
	if (Phase < 0 || Phase >= 4) return;

	GoalBarText->SetText(FText::FromString(GoalBarTexts[Phase]));
	GoalText->SetText(FText::FromString(GoalTexts[Phase]));

	CurrentPhase = Phase;

	if (Phase == 1)
	{
		Raider->SetVisibility(ESlateVisibility::Visible);
		LaboA->SetVisibility(ESlateVisibility::Visible);
		LaboB->SetVisibility(ESlateVisibility::Visible);
	}
	else if (Phase == 3)
	{
		Oasis->SetVisibility(ESlateVisibility::Visible);
	}
}

void UInGameHUD::SetBuildings(APlayerCharacter* Ch, FVector LaboAPos, FVector LaboBPos, FVector OasisPos)
{
	if (!Ch || !Ch->GetController()) return;

	Player = Ch;
	PlayerCamera = Player->FindComponentByClass<UCameraComponent>();
	PlayerSpringArm = Player->FindComponentByClass<USpringArmComponent>();

	LaboALoc = LaboAPos;
	LaboBLoc = LaboBPos;
	OasisLoc = OasisPos;
}

float UInGameHUD::GetAngleToTarget(const FVector& TargetLoc)
{
	FVector Forward = PlayerSpringArm->GetComponentLocation() - PlayerCamera->GetComponentLocation();
	Forward.Z = 0;
	Forward = Forward.GetSafeNormal2D();

	FVector Dir = (TargetLoc - Player->GetActorLocation());
	Dir.Z = 0;
	Dir = Dir.GetSafeNormal2D();

	float Dot = FVector::DotProduct(Forward, Dir);
	Dot = FMath::Clamp(Dot, -1.0f, 1.0f);

	float Angle = FMath::Acos(Dot) * (180.0f / PI);

	float CrossZ = Forward.X * Dir.Y - Forward.Y * Dir.X;
	if (CrossZ < 0)
	{
		Angle = 360.f - Angle;
	}

	return Angle;
}

void UInGameHUD::ApplyMarkerRotation(UImage* Marker, float Angle)
{
	UE_LOG(LogTemp, Warning, TEXT("Applying"));

	if (!Marker) return;

	UE_LOG(LogTemp, Warning, TEXT("Applying Rotation: %f"), Angle);

	FWidgetTransform Transform = Marker->GetRenderTransform();
	Transform.Angle = Angle;
	Marker->SetRenderTransform(Transform);
}