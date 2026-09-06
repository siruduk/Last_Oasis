
#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Character.h"
#include "EnemyAnim.h"
#include "../Character/PlayerCharacter.h"
#include "EnemyCh.generated.h"

UCLASS()
class LAST_OASIS_API AEnemyCh : public ACharacter
{
	GENERATED_BODY()

public:
	AEnemyCh();

protected:
	virtual void BeginPlay() override;

public:	

public:
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	class UEnemyAnim* Anim;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	FVector GoalPos;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	float HP;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	float MaxHP;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	APlayerCharacter* Player;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	float AttackRange;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	float ChaseRange;

	UFUNCTION(BlueprintCallable,BlueprintImplementableEvent)
	void DecreaseHP(float Value);
protected:
	
};
