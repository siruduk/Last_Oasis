// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "AttributeSet.h"
#include "AbilitySystemComponent.h"
#include "LOAttributeSet.generated.h"

#define ATTRIBUTE_ACCESSORS(ClassName, PropertyName) \
GAMEPLAYATTRIBUTE_PROPERTY_GETTER(ClassName, PropertyName) \
GAMEPLAYATTRIBUTE_VALUE_GETTER(PropertyName) \
GAMEPLAYATTRIBUTE_VALUE_SETTER(PropertyName) \
GAMEPLAYATTRIBUTE_VALUE_INITTER(PropertyName)


/**
 * 
 */
UCLASS()
class LAST_OASIS_API ULOAttributeSet : public UAttributeSet
{
	GENERATED_BODY()
public:

	ULOAttributeSet();
	ATTRIBUTE_ACCESSORS(ULOAttributeSet, MaxHealth);
	ATTRIBUTE_ACCESSORS(ULOAttributeSet, Health);
	ATTRIBUTE_ACCESSORS(ULOAttributeSet, MaxHunger);
	ATTRIBUTE_ACCESSORS(ULOAttributeSet, Hunger);
	ATTRIBUTE_ACCESSORS(ULOAttributeSet, MaxThirst);
	ATTRIBUTE_ACCESSORS(ULOAttributeSet, Thirst);
	ATTRIBUTE_ACCESSORS(ULOAttributeSet, MaxTemperature);
	ATTRIBUTE_ACCESSORS(ULOAttributeSet, MinTemperature);
	ATTRIBUTE_ACCESSORS(ULOAttributeSet, Temperature);
	ATTRIBUTE_ACCESSORS(ULOAttributeSet, Level);
	ATTRIBUTE_ACCESSORS(ULOAttributeSet, Damage);
	ATTRIBUTE_ACCESSORS(ULOAttributeSet, Speed);

protected:
	virtual void PreAttributeChange(const FGameplayAttribute& Attribute, float& NewValue) override;
	virtual bool PreGameplayEffectExecute(struct FGameplayEffectModCallbackData& Data) override;
	virtual void PostGameplayEffectExecute(const struct FGameplayEffectModCallbackData& Data) override;

protected:
	UPROPERTY(BlueprintReadOnly, Category = "Stat", meta = (AllowPrivateAccess = true))
	FGameplayAttributeData MaxHealth;
	UPROPERTY(BlueprintReadOnly, Category = "Stat", meta = (AllowPrivateAccess = true))
	FGameplayAttributeData Health;
	UPROPERTY(BlueprintReadOnly, Category = "Stat", meta = (AllowPrivateAccess = true))
	FGameplayAttributeData MaxHunger;
	UPROPERTY(BlueprintReadOnly, Category = "Stat", meta = (AllowPrivateAccess = true))
	FGameplayAttributeData Hunger;
	UPROPERTY(BlueprintReadOnly, Category = "Stat", meta = (AllowPrivateAccess = true))
	FGameplayAttributeData MaxThirst;
	UPROPERTY(BlueprintReadOnly, Category = "Stat", meta = (AllowPrivateAccess = true))
	FGameplayAttributeData Thirst;
	UPROPERTY(BlueprintReadOnly, Category = "Stat", meta = (AllowPrivateAccess = true))
	FGameplayAttributeData MaxTemperature;
	UPROPERTY(BlueprintReadOnly, Category = "Stat", meta = (AllowPrivateAccess = true))
	FGameplayAttributeData MinTemperature;
	UPROPERTY(BlueprintReadWrite, Category = "Stat", meta = (AllowPrivateAccess = true))
	FGameplayAttributeData Temperature;
	UPROPERTY(BlueprintReadOnly, Category = "Stat", meta = (AllowPrivateAccess = true))
	FGameplayAttributeData Level;
	UPROPERTY(BlueprintReadOnly, Category = "Stat", meta = (AllowPrivateAccess = true))
	FGameplayAttributeData Damage;
	UPROPERTY(BlueprintReadOnly, Category = "Stat", meta = (AllowPrivateAccess = true))
	FGameplayAttributeData Speed;
};
