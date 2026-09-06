

#include "MainGameMode.h"
#include "Kismet/GameplayStatics.h"
#include "Camera/CameraActor.h"
#include "CineCameraActor.h"
#include "GameFramework/PlayerController.h"

AMainGameMode::AMainGameMode()
{
	DefaultPawnClass = nullptr;

}

void AMainGameMode::BeginPlay()
{
	Super::BeginPlay();

    APlayerController* PC = UGameplayStatics::GetPlayerController(this, 0);
    if (PC)
    {
        AActor* FoundCamera = UGameplayStatics::GetActorOfClass(GetWorld(), ACineCameraActor::StaticClass());

        if (FoundCamera)
        {
            ACineCameraActor* CineCam = Cast<ACineCameraActor>(FoundCamera);
            if (CineCam)
            {
                PC->SetViewTarget(CineCam);
            }
        }
    }
}