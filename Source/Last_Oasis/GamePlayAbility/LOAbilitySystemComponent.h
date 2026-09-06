// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "AbilitySystemComponent.h"
#include "AbilitySystemInterface.h"
#include "GamePlayAbility/AttributeSet/LOAttributeSet.h"
#include "LOAbilitySystemComponent.generated.h"

/**
 * 
 */
UCLASS()
class LAST_OASIS_API ULOAbilitySystemComponent : public UAbilitySystemComponent
{
	GENERATED_BODY()
public:
	void InitializeComponent() override;
	void RemoveTagsByCategory(FGameplayTag CategoryTag);

protected:
	void OnHealthChanged(const FOnAttributeChangeData& Data);
	void OnHungerChanged(const FOnAttributeChangeData& Data);
	void OnThirstChanged(const FOnAttributeChangeData& Data);
	void OnTemperatureChanged(const FOnAttributeChangeData& Data);

	friend class APlayerCharacter;
};
