
#pragma once

#include "CoreMinimal.h"
#include "GameFramework/GameModeBase.h"
#include "MainGameMode.generated.h"

UCLASS()
class LAST_OASIS_API AMainGameMode : public AGameModeBase
{
	GENERATED_BODY()
	
public:
	AMainGameMode();

protected:
	virtual void BeginPlay() override;
};
