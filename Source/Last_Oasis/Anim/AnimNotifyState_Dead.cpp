// Fill out your copyright notice in the Description page of Project Settings.


#include "AnimNotifyState_Dead.h"
#include "Character/PlayerCharacter.h"


void UAnimNotifyState_Dead::NotifyBegin(USkeletalMeshComponent* MeshComp, UAnimSequenceBase* Animation,
                                        float TotalDuration, const FAnimNotifyEventReference& EventReference)
{
	if (APlayerCharacter* Character = Cast<APlayerCharacter>(MeshComp->GetOwner()))
	{
		//fade
	}
}

void UAnimNotifyState_Dead::NotifyEnd(USkeletalMeshComponent* MeshComp, UAnimSequenceBase* Animation,
	const FAnimNotifyEventReference& EventReference)
{

}
