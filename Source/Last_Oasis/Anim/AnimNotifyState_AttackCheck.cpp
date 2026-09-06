// Fill out your copyright notice in the Description page of Project Settings.


#include "Anim/AnimNotifyState_AttackCheck.h"
#include "Character/PlayerCharacter.h"

void UAnimNotifyState_AttackCheck::NotifyBegin(USkeletalMeshComponent* MeshComp, UAnimSequenceBase* Animation,
                                               float TotalDuration, const FAnimNotifyEventReference& EventReference)
{
	if (APlayerCharacter* Character = Cast<APlayerCharacter>(MeshComp->GetOwner()))
	{
		Character->SetWeaponRange(Value);
	}
}

void UAnimNotifyState_AttackCheck::NotifyEnd(USkeletalMeshComponent* MeshComp, UAnimSequenceBase* Animation,
	const FAnimNotifyEventReference& EventReference)
{
	if (APlayerCharacter* Character = Cast<APlayerCharacter>(MeshComp->GetOwner()))
	{
		Character->SetWeaponRange(0);
		Character->EndHitCheck();
	}
}
