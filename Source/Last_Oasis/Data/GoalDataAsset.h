
#pragma once

#include "CoreMinimal.h"
#include "Engine/DataAsset.h"
#include "GoalDataAsset.generated.h"

UCLASS()
class LAST_OASIS_API UGoalDataAsset : public UDataAsset
{
	GENERATED_BODY()
	
public:
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Goal")
	TArray<FString> GoalTextArray;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Goal")
	TArray<FString> GoalBarTextArray;
};
