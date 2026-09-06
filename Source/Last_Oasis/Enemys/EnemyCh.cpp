

#include "EnemyCh.h"
#include "Kismet/GameplayStatics.h"

AEnemyCh::AEnemyCh()
{
	PrimaryActorTick.bCanEverTick = true;

}

void AEnemyCh::BeginPlay()
{
	Super::BeginPlay();

	Anim = Cast<UEnemyAnim>(GetMesh()->GetAnimInstance());

	AActor* FoundActor = UGameplayStatics::GetActorOfClass(GetWorld(), APlayerCharacter::StaticClass());
	Player = Cast<APlayerCharacter>(FoundActor);
}

