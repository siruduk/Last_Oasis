// Fill out your copyright notice in the Description page of Project Settings.


#include "GamePlayAbility/AttributeSet/LOAttributeSet.h"
#include "GameplayEffectExtension.h"

ULOAttributeSet::ULOAttributeSet() :
	MaxHealth(100.f),
	Health(50.f),
	MaxHunger(100.f),
	Hunger(50.f),
	MaxThirst(100.f),
	Thirst(50.f),
	MaxTemperature(100.f),
	MinTemperature(0.f),
	Temperature(36.5f),
	Level(1.f),
	Damage(20.f),
	Speed(550.f)
{
	
}

void ULOAttributeSet::PreAttributeChange(const FGameplayAttribute& Attribute, float& NewValue)
{

}

bool ULOAttributeSet::PreGameplayEffectExecute(struct FGameplayEffectModCallbackData& Data)
{
	if (Data.Target.HasMatchingGameplayTag(FGameplayTag::RequestGameplayTag("State.Dead")))
	{
		return false;
	}
	if (Data.Target.HasMatchingGameplayTag(FGameplayTag::RequestGameplayTag(FName("State.Warm"))) &&
		Data.Target.HasMatchingGameplayTag(FGameplayTag::RequestGameplayTag(FName("Game.Night"))))
	{
		if (Data.EvaluatedData.Attribute.AttributeName == GetTemperatureAttribute().AttributeName)
		{
			if (Temperature.GetCurrentValue() >= 36.5)
			{
				Data.EvaluatedData.Magnitude = 0;
			}
			else
			{
				Data.EvaluatedData.Magnitude = 0.01;
			}
		}
	}

	if (Data.EvaluatedData.Attribute.AttributeName == GetThirstAttribute().AttributeName &&
		Data.Target.HasMatchingGameplayTag(FGameplayTag::RequestGameplayTag(FName("Game.Day"))) &&
		Data.Target.HasMatchingGameplayTag(FGameplayTag::RequestGameplayTag(FName("State.InShadow"))))
	{
		if (!FMath::IsNearlyEqual(Data.EvaluatedData.Magnitude, 10.f))
			Data.EvaluatedData.Magnitude *= 0.75;
	}

	if (Data.EvaluatedData.Attribute.AttributeName == GetTemperatureAttribute().AttributeName &&
	Data.Target.HasMatchingGameplayTag(FGameplayTag::RequestGameplayTag(FName("Game.Day"))) &&
	Data.Target.HasMatchingGameplayTag(FGameplayTag::RequestGameplayTag(FName("State.Thirst.Hydrated"))))
	{
		if (!FMath::IsNearlyEqual(Data.EvaluatedData.Magnitude, 0.2f))
			Data.EvaluatedData.Magnitude *= 0.5;
	}
	
	if (Data.EvaluatedData.Attribute.AttributeName == GetTemperatureAttribute().AttributeName &&
		Data.Target.HasMatchingGameplayTag(FGameplayTag::RequestGameplayTag(FName("Game.Day"))) &&
		Data.Target.HasMatchingGameplayTag(FGameplayTag::RequestGameplayTag(FName("State.InShadow"))))
	{
		Data.EvaluatedData.Magnitude *= -0.1;
	}
	
	return true;
}

void ULOAttributeSet::PostGameplayEffectExecute(const struct FGameplayEffectModCallbackData& Data)
{
	Super::PostGameplayEffectExecute(Data);
	if (Data.EvaluatedData.Attribute.AttributeName == GetHealthAttribute().AttributeName)
	{
		SetHealth(FMath::Clamp(GetHealth(), 0, GetMaxHealth()));
		if (GetHealth() == 0 && !Data.Target.HasMatchingGameplayTag(FGameplayTag::RequestGameplayTag("State.Dead")))
		{
			Data.Target.AddLooseGameplayTag(FGameplayTag::RequestGameplayTag("State.Dead"));
			Data.Target.RemoveLooseGameplayTag(FGameplayTag::RequestGameplayTag("State.CanAttack"));
		}
	}

	if (Data.EvaluatedData.Attribute.AttributeName == GetThirstAttribute().AttributeName)
	{
		SetThirst(FMath::Clamp(GetThirst(), 0, GetMaxThirst()));
	}

	if (Data.EvaluatedData.Attribute.AttributeName == GetHungerAttribute().AttributeName)
	{
		SetHunger(FMath::Clamp(GetHunger(), 0, GetMaxHunger()));
	}
}
