// Fill out your copyright notice in the Description page of Project Settings.


#include "GamePlayAbility/GameplayAbility/LOGA_Attack.h"
#include "Abilities/Tasks/AbilityTask_PlayMontageAndWait.h"
#include "Character/PlayerCharacter.h"
#include "Data/AttackData.h"
#include "GameFramework/CharacterMovementComponent.h"

void ULOGA_Attack::ActivateAbility(const FGameplayAbilitySpecHandle Handle, const FGameplayAbilityActorInfo* ActorInfo,
                                   const FGameplayAbilityActivationInfo ActivationInfo, const FGameplayEventData* TriggerEventData)
{
	Super::ActivateAbility(Handle, ActorInfo, ActivationInfo, TriggerEventData);
	APlayerCharacter *Character = CastChecked<APlayerCharacter>(ActorInfo->AvatarActor.Get());
	Character->GetCharacterMovement()->SetMovementMode(MOVE_None);
	
	UAbilityTask_PlayMontageAndWait* PlayAttackTask = UAbilityTask_PlayMontageAndWait::CreatePlayMontageAndWaitProxy(this, TEXT("Attack"), Character->GetAttackMontage(), 1.0f, GetNextSection());
	PlayAttackTask->OnCompleted.AddDynamic(this,&ULOGA_Attack::OnCompleteCallback);
	PlayAttackTask->OnInterrupted.AddDynamic(this, &ULOGA_Attack::OnInterruptedCallback);
	PlayAttackTask->ReadyForActivation();

	StartComboTimer();
}

void ULOGA_Attack::CancelAbility(const FGameplayAbilitySpecHandle Handle, const FGameplayAbilityActorInfo* ActorInfo,
	const FGameplayAbilityActivationInfo ActivationInfo, bool bReplicateCancelAbility)
{
	Super::CancelAbility(Handle, ActorInfo, ActivationInfo, bReplicateCancelAbility);
}

void ULOGA_Attack::EndAbility(const FGameplayAbilitySpecHandle Handle, const FGameplayAbilityActorInfo* ActorInfo,
	const FGameplayAbilityActivationInfo ActivationInfo, bool bReplicateEndAbility, bool bWasCancelled)
{
	Super::EndAbility(Handle, ActorInfo, ActivationInfo, bReplicateEndAbility, bWasCancelled);
	APlayerCharacter *Character = CastChecked<APlayerCharacter>(ActorInfo->AvatarActor.Get());
	Character->GetCharacterMovement()->SetMovementMode(MOVE_Walking);
	
	CurrentCombo = 0;
	ComboData = nullptr;
	HasNextcomboInput = false;
}

void ULOGA_Attack::InputPressed(const FGameplayAbilitySpecHandle Handle, const FGameplayAbilityActorInfo* ActorInfo,
	const FGameplayAbilityActivationInfo ActivationInfo)
{
	Super::InputPressed(Handle, ActorInfo, ActivationInfo);
	if (!ComboTimerHandle.IsValid())
	{
		HasNextcomboInput = false;
	}
	else
	{
		HasNextcomboInput = true;
	}
}

bool ULOGA_Attack::CanActivateAbility(const FGameplayAbilitySpecHandle Handle,
	const FGameplayAbilityActorInfo* ActorInfo, const FGameplayTagContainer* SourceTags,
	const FGameplayTagContainer* TargetTags, FGameplayTagContainer* OptionalRelevantTags) const
{
	ACharacter* Character = Cast<ACharacter>(ActorInfo->AvatarActor.Get());
	if (Character && Character->GetCharacterMovement()->IsFalling())
	{
		return false;
	}
	return Super::CanActivateAbility(Handle, ActorInfo, SourceTags, TargetTags, OptionalRelevantTags);
}

void ULOGA_Attack::OnCompleteCallback()
{
	bool bReplicatedEndAblity = true;
	bool bWasCancelled = false;

	EndAbility(CurrentSpecHandle, CurrentActorInfo, CurrentActivationInfo, bReplicatedEndAblity, bWasCancelled);
}

void ULOGA_Attack::OnInterruptedCallback()
{
	bool bReplicatedEndAblity = true;
	bool bWasCancelled = true;
	UE_LOG(LogTemp, Log, TEXT("Inturrepted"));

	EndAbility(CurrentSpecHandle, CurrentActorInfo, CurrentActivationInfo, bReplicatedEndAblity, bWasCancelled);
}

FName ULOGA_Attack::GetNextSection()
{
	CurrentCombo = FMath::Clamp(CurrentCombo + 1, 1, ComboData->MaxComboCount);
	FName NextSection = *FString::Printf(TEXT("%s%d"),*ComboData->MontageSectionNamePrefix, CurrentCombo);
	return NextSection;
}

void ULOGA_Attack::StartComboTimer()
{
	int32 ComboIndex = CurrentCombo -1;
	ensure(ComboData->EffectiveFrameCount.IsValidIndex(ComboIndex));

	float ComboEffectiveTime = ComboData->EffectiveFrameCount[ComboIndex] / ComboData->FrameRate;
	if (ComboEffectiveTime > 0.0f)
	{
		GetWorld()->GetTimerManager().SetTimer(ComboTimerHandle, this, &ULOGA_Attack::CheckComboInput, ComboEffectiveTime,false);
	}
}

void ULOGA_Attack::CheckComboInput()
{
	ComboTimerHandle.Invalidate();
	if (HasNextcomboInput)
	{
		MontageJumpToSection(GetNextSection());
		StartComboTimer();
		HasNextcomboInput = false;
	}
}
