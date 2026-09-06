// Fill out your copyright notice in the Description page of Project Settings.


#include "../Actor/CraftingManager.h"

#include "Character/PlayerCharacter.h"
#include "GameFramework/PlayerController.h"
#include "Kismet/GameplayStatics.h"

#include "GamePlayAbility/LOAbilitySystemComponent.h"

// Sets default values
ACraftingManager::ACraftingManager()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

}

// Called when the game starts or when spawned
void ACraftingManager::BeginPlay()
{
	Super::BeginPlay();
	
}

// Called every frame
void ACraftingManager::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

void ACraftingManager::StoryItemCraftingEvent(int32 index)
{
	UE_LOG(LogTemp, Warning, TEXT("Crafting Event Index : %d"), index);
	OnCraftingEvent.Broadcast(index);
}

void ACraftingManager::WeaponCrafting()
{
	 PC = UGameplayStatics::GetPlayerController(GetWorld(), 0);
	if (PC)
	{
		APlayerCharacter* PlayerChar = Cast<APlayerCharacter>(PC->GetPawn());
		if (PlayerChar)
			PlayerChar->GetWeapon();
	}
	UE_LOG(LogTemp, Warning, TEXT("Weapon Crafting Event"));
}


void ACraftingManager::DuskMaskCrafting()
{
	APlayerController* Controller = GetWorld()->GetFirstPlayerController();
	if (Controller)
	{
		Cast<IAbilitySystemInterface>(Controller->GetPawn())->GetAbilitySystemComponent()->AddLooseGameplayTag(FGameplayTag::RequestGameplayTag("State.Mask"));
	}
	UE_LOG(LogTemp, Warning, TEXT("Weapon Crafting Event"));
}



