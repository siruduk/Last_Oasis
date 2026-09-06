// Fill out your copyright notice in the Description page of Project Settings.


#include "Player/LOPlayerState.h"
#include "AbilitySystemComponent.h"
#include "GamePlayAbility/LOAbilitySystemComponent.h"
#include "GamePlayAbility/AttributeSet/LOAttributeSet.h"

void ALOPlayerState::InitASC()
{
	ASC = NewObject<ULOAbilitySystemComponent>(this, ULOAbilitySystemComponent::StaticClass(), TEXT("ASC"));
	AttributeSet = NewObject<ULOAttributeSet>(this, ULOAttributeSet::StaticClass(), TEXT("AttributeSet"));
	ASC->AddAttributeSetSubobject(AttributeSet.Get());
	ASC->RegisterComponent();
	ASC -> SetIsReplicated(true);
}

ALOPlayerState::ALOPlayerState()
{
}

class UAbilitySystemComponent* ALOPlayerState::GetAbilitySystemComponent() const
{
	return ASC;
}
