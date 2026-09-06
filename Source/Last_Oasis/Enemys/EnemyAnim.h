
#pragma once

#include "CoreMinimal.h"
#include "Animation/AnimInstance.h"
#include "EnemyAnim.generated.h"

UCLASS()
class LAST_OASIS_API UEnemyAnim : public UAnimInstance
{
	GENERATED_BODY()
	
protected:
	virtual void NativeBeginPlay() override; 

public:
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	int32 AnimStateIdx;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	bool bAnimAttack;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	bool bAnimDead;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	float Speed;
};
