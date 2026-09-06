// Fill out your copyright notice in the Description page of Project Settings.


#include "GameMode/LOGameModeBase.h"
#include "Engine/DirectionalLight.h"
#include "Kismet/GameplayStatics.h"
#include "UI/InGameHUD.h"
#include "EngineUtils.h"
#include "Math/UnrealMathUtility.h"
#include "Actor/BuildingPoint.h"
#include "Kismet/KismetMathLibrary.h"

ALOGameModeBase::ALOGameModeBase()
{
    PrimaryActorTick.bCanEverTick = true;
}

void ALOGameModeBase::Respawn()
{
    APlayerController* Controller = GetWorld()->GetFirstPlayerController();
    if (Controller)
    {
        APawn* NewPawn = SpawnDefaultPawnAtTransform(Controller, SpawnPoint);
        if (NewPawn)
        {
            Controller->Possess(NewPawn);
            SandStorm = false;
            AddMinute(360);
        }
    }
}

    void ALOGameModeBase::SetSpawnPoint(FTransform SpawnTransform)
{
    SpawnPoint = SpawnTransform;
    SpawnPoint += FTransform(FVector(0,-500,0));
    SpawnPoint.SetScale3D(FVector(1,1,1));
}

void ALOGameModeBase::SetTimeOfDay(int32 NewHour, int32 NewMinute)
{
    NewHour = FMath::Clamp(NewHour, 0, 23);
    NewMinute = FMath::Clamp(NewMinute, 0, 59);

    float TargetHour = NewHour + (NewMinute / 60.0f);

    float CurrentHourNow = (ElapsedTime / DayLength) * 24.0f;

    if (CurrentHourNow < 12.0f && TargetHour >= 0.0f)
    {
        Days++;
        if (PC && PC->HUD)
        {
            PC->HUD->UpdateDays(Days);
        }
    }

    ElapsedTime = (TargetHour / 24.0f) * DayLength;

    CurrentHour = TargetHour;

    if (Sun)
    {
        float Pitch = (CurrentHour / 24.0f) * 360.0f - 90.0f;
        FRotator NewRotation(-Pitch, -90.0f, 0.0f);
        Sun->SetActorRotation(NewRotation);
    }

    if (PC && PC->HUD)
    {
        PC->HUD->UpdateTime(NewHour, NewMinute);
    }
}

bool ALOGameModeBase::GetRemainTimeUntilMorning()
{
    float NextMorning = 6.0f;
    float TimeLeft;

    if (CurrentHour < NextMorning)
    {
        TimeLeft = NextMorning - CurrentHour;
    }
    else
    {
        TimeLeft = 24.0f - CurrentHour + NextMorning;
    }
    return TimeLeft >= 5.0f;
}

void ALOGameModeBase::AddMinute(int32 AddMinute)
{
    if (AddMinute <= 0) return;

    float HoursToAdd = AddMinute / 60.0f;

    float TimeToAdd = (HoursToAdd / 24.0f) * DayLength;

    ElapsedTime += TimeToAdd;

    if (ElapsedTime > DayLength)
    {
        ElapsedTime -= DayLength;
        Days++;

        if (PC && PC->HUD)
        {
            PC->HUD->UpdateDays(Days);
        }
    }

    CurrentHour = (ElapsedTime / DayLength) * 24.0f;
    int32 Hour = FMath::FloorToInt(CurrentHour);
    int32 Minute = FMath::FloorToInt((CurrentHour - Hour) * 60.0f);

    if (Sun)
    {
        float Pitch = (CurrentHour / 24.0f) * 360.0f - 90.0f;
        FRotator NewRotation(-Pitch, -90.0f, 0.0f);
        Sun->SetActorRotation(NewRotation);
    }

    if (PC && PC->HUD)
    {
        PC->HUD->UpdateTime(Hour, Minute);
    }
}

void ALOGameModeBase::BeginPlay()
{
	Super::BeginPlay();
    Sun = Cast<ADirectionalLight>(UGameplayStatics::GetActorOfClass(GetWorld(), ADirectionalLight::StaticClass()));
    ElapsedTime = (6.f / 24.0f) * DayLength;
	//GetWorld()->GetTimerManager().SetTimer(TimeHandle, this, &ALOGameModeBase::UpdateGameTime, 1.0f, true);

    PC = Cast<ALOPlayerController>(UGameplayStatics::GetPlayerController(this, 0));
        
	SpawnBuilding();

	FTimerHandle TempHandle;
    //GetWorld()->GetTimerManager().SetTimer(TempHandle, this, &ALOGameModeBase::SpawnFarm, 0.5f, true);
}

void ALOGameModeBase::Tick(float DeltaSeconds)
{
    Super::Tick(DeltaSeconds);
    ElapsedTime += DeltaSeconds;

    if (ElapsedTime > DayLength)
    {
        ElapsedTime -= DayLength;

        Days++;
        if (PC->HUD)
        {
            PC->HUD->UpdateDays(Days);
        }
    }

    CurrentHour = (ElapsedTime / DayLength) * 24.0f;

    int32 Hour = FMath::FloorToInt(CurrentHour);
    int32 Minute = FMath::FloorToInt((CurrentHour - Hour) * 60.0f);

    if (Minute != LastPrintedMinute)
    {
        LastPrintedMinute = Minute;

        if (Minute == 0 && Days > 4)
        {
            if (!SandStorm)
            {
                if (FMath::FRand() < 0.05f)
                {
                    APlayerController* Controller = GetWorld()->GetFirstPlayerController();
                    if (Controller)
                    {
                        SandStorm = true;
                        Cast<IAbilitySystemInterface>(Controller->GetPawn())->GetAbilitySystemComponent()->AddLooseGameplayTag(FGameplayTag::RequestGameplayTag("Game.SandStorm"));
                    }
                }
            }
            else
            {
                if (FMath::FRand() < 0.4f)
                {
                    APlayerController* Controller = GetWorld()->GetFirstPlayerController();
                    if (Controller)
                    {
                        SandStorm = false;
                        Cast<IAbilitySystemInterface>(Controller->GetPawn())->GetAbilitySystemComponent()->RemoveLooseGameplayTag(FGameplayTag::RequestGameplayTag("Game.SandStorm"));
                    }
                }
            }
        }
        if(PC->HUD)
        {
            PC->HUD->UpdateTime(Hour, Minute);
		}
    }
    if (Sun == nullptr) return;

    float Pitch = (CurrentHour / 24.0f) * 360.0f - 90.0f;

    FRotator NewRotation(-Pitch, -90.0f, 0.0f);
    Sun->SetActorRotation(NewRotation);
}

void ALOGameModeBase::SpawnBuilding()
{
    TArray<AActor*> FoundActors;
    UGameplayStatics::GetAllActorsOfClass(GetWorld(), ABuildingPoint::StaticClass(), FoundActors);

    if (!GetWorld() || !PC || !PC->HUD || FoundActors.Num() < 3)
    {
        UE_LOG(LogTemp, Warning, TEXT("Build"));

        GetWorld()->GetTimerManager().SetTimer(HUDCheckTimerHandle, this, &ALOGameModeBase::SpawnBuilding, 0.1f, false);
        return;
    }

    TArray<FVector> Locations;

    UE_LOG(LogTemp, Warning, TEXT("Build %d"), FoundActors.Num());

    for (int32 i = 0; i < FoundActors.Num(); ++i)
    {
        int32 SwapIndex = FMath::RandRange(i, FoundActors.Num() - 1);
        FoundActors.Swap(i, SwapIndex);
    }

    AActor* LaboA = nullptr;

    for (int32 i = 0; i < 3; ++i)
    {
        Locations.Add(FoundActors[i]->GetActorLocation());

        AActor* Spawned = nullptr;

        if (i == 0 && BuildingA)
        {
            LaboA = GetWorld()->SpawnActor<AActor>(BuildingA, Locations[0], FRotator::ZeroRotator);
        }
        else if (i == 1 && BuildingB)
        {
            Spawned = GetWorld()->SpawnActor<AActor>(BuildingB, Locations[1], FRotator::ZeroRotator);
        }
        else if (i == 2 && BuildingC)
        {
            Spawned = GetWorld()->SpawnActor<AActor>(BuildingC, Locations[2], FRotator::ZeroRotator);
        }

        if (Spawned)
        {
            SpawnedBuildings.Add(Spawned);
        }
    }

    SpawnPoint = LaboA->GetActorTransform();
    SpawnPoint += FTransform(FVector(0, -1000, +200));

	SpawnPoint.SetScale3D(FVector(1, 1, 1));

    APlayerController* Controller = GetWorld()->GetFirstPlayerController();
    if (Controller)
    {
        Controller->GetPawn()->SetActorTransform(SpawnPoint);
    }

    if (PC->HUD)
    {
        UE_LOG(LogTemp, Warning, TEXT("SetBuilding"));
        PC->HUD->SetBuildings(PC->GetPawn<APlayerCharacter>(), Locations[0], Locations[1], Locations[2]);
    }
    else
    {
        UE_LOG(LogTemp, Warning, TEXT("NoHUD"));
    }
}

void ALOGameModeBase::SpawnFarm()
{
    TArray<TSubclassOf<AActor>> SpawnPool;

    SpawnPool.Append(TArray<TSubclassOf<AActor>>{ ActorClass1, ActorClass1, ActorClass1, ActorClass1 });
    SpawnPool.Append(TArray<TSubclassOf<AActor>>{ ActorClass2, ActorClass2, ActorClass2, ActorClass2 });
    SpawnPool.Append(TArray<TSubclassOf<AActor>>{ ActorClass3, ActorClass3 });
    SpawnPool.Append(TArray<TSubclassOf<AActor>>{ ActorClass4 });
    SpawnPool.Append(TArray<TSubclassOf<AActor>>{ ActorClass5 });


    TArray<FVector> SpawnedLocations;
    float MinDistance = 500.0f;
    int32 MaxSpawn = 100;
    int32 MaxTries = 50;

    for (int32 i = 0; i < MaxSpawn; i++)
    {
        FVector RandomLoc;
        bool bFoundValidLocation = false;

        for (int32 Try = 0; Try < MaxTries; Try++)
        {
            RandomLoc = UKismetMathLibrary::RandomPointInBoundingBox(BoxCenter, BoxExtent);

            RandomLoc.Z += 12000;

            bool bTooClose = false;
            for (const FVector& Loc : SpawnedLocations)
            {
                if (FVector::DistXY(Loc, RandomLoc) < MinDistance)
                {
                    bTooClose = true;
                    break;
                }
            }

            if (bTooClose)
                continue;

            FHitResult Hit;
            FCollisionQueryParams Params;
            Params.bTraceComplex = true;

            bool bHit = GetWorld()->LineTraceSingleByChannel(
                Hit,
                RandomLoc,
                FVector(RandomLoc.X, RandomLoc.Y, RandomLoc.Z - 5000.0f),
                ECC_WorldStatic,
                Params
            );

            FColor LineColor = bHit ? FColor::Green : FColor::Red;
            DrawDebugLine(
                GetWorld(),
                RandomLoc,
                bHit ? Hit.Location : FVector(RandomLoc.X, RandomLoc.Y, RandomLoc.Z - 5000.0f),
                LineColor,
                false,       // ���� �ð�
                5.0f,        // ���� ���� �ð� 5��
                0,           // ���� �켱 ����
                5.0f         // �β�
            );

            if (bHit)
            {
                RandomLoc.Z = Hit.Location.Z;
                bFoundValidLocation = true;
                break;
            }
        }		

        if (bFoundValidLocation)
        {
            int32 RandIndex = FMath::RandRange(0, SpawnPool.Num() - 1);
            TSubclassOf<AActor> SelectedClass = SpawnPool[RandIndex];

            if (SelectedClass)
            {
                FActorSpawnParameters SpawnParams;
                SpawnParams.SpawnCollisionHandlingOverride = ESpawnActorCollisionHandlingMethod::AdjustIfPossibleButAlwaysSpawn;

                AActor* Spawned = GetWorld()->SpawnActor<AActor>(SelectedClass, RandomLoc, FRotator::ZeroRotator, SpawnParams);
                if (Spawned)
                {
                    SpawnedLocations.Add(RandomLoc);
                }
            }
        }
        else
        {
            UE_LOG(LogTemp, Warning, TEXT("Failed to find valid location for spawn %d"), i);
        }
    }
}

void ALOGameModeBase::UpdateGameTime()
{
	ElapsedTime += 1.0f;

	if (ElapsedTime > DayLength)
	{
		ElapsedTime -= DayLength;
	}
	CurrentHour = (ElapsedTime / DayLength) * 24.0f;
}

