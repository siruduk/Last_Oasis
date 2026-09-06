// Fill out your copyright notice in the Description page of Project Settings.


#include "Character/PlayerCharacter.h"

#include "AbilitySystemComponent.h"
#include "InputMappingContext.h"
#include "EnhancedInputComponent.h"
#include "EnhancedInputSubsystems.h"
#include "Camera/CameraComponent.h"
#include "Components/CapsuleComponent.h"
#include "Engine/DirectionalLight.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "GameFramework/SpringArmComponent.h"
#include "GamePlayAbility/LOAbilitySystemComponent.h"
#include "Kismet/GameplayStatics.h"
#include "Player/LOPlayerState.h"
#include "../Data/InventoryItemStruct.h"
#include "../Actor/InventoryManager.h"
#include "Components/BoxComponent.h"
#include "Enemys/EnemyCh.h"
#include "GameMode/LOGameModeBase.h"
#include "Kismet/KismetMathLibrary.h"
#include "DrawDebugHelpers.h"
#include "UI/InGameHUD.h"
#include "Enemys/EnemyAnim.h"


// Sets default values
APlayerCharacter::APlayerCharacter()
{
    PrimaryActorTick.bCanEverTick = true;
    // Set size for collision capsule
    GetCapsuleComponent()->InitCapsuleSize(42.f, 96.0f);
		
    // Don't rotate when the controller rotates. Let that just affect the camera.
    bUseControllerRotationPitch = false;
    bUseControllerRotationYaw = false;
    bUseControllerRotationRoll = false;

    // Configure character movement
    GetCharacterMovement()->bOrientRotationToMovement = true; // Character moves in the direction of input...	
    GetCharacterMovement()->RotationRate = FRotator(0.0f, 500.0f, 0.0f); // ...at this rotation rate

    // Note: For faster iteration times these variables, and many more, can be tweaked in the Character Blueprint
    // instead of recompiling to adjust them
    GetCharacterMovement()->JumpZVelocity = 700.f;
    GetCharacterMovement()->AirControl = 0.35f;
    GetCharacterMovement()->MaxWalkSpeed = 500.f;
    GetCharacterMovement()->MinAnalogWalkSpeed = 20.f;
    GetCharacterMovement()->BrakingDecelerationWalking = 2000.f;
    GetCharacterMovement()->BrakingDecelerationFalling = 1500.0f;

    // Create a camera boom (pulls in towards the player if there is a collision)
    CameraBoom = CreateDefaultSubobject<USpringArmComponent>(TEXT("CameraBoom"));
    CameraBoom->SetupAttachment(RootComponent);
    CameraBoom->TargetArmLength = 400.0f; // The camera follows at this distance behind the character	
    CameraBoom->bUsePawnControlRotation = true; // Rotate the arm based on the controller

    // Create a follow camera
    FollowCamera = CreateDefaultSubobject<UCameraComponent>(TEXT("FollowCamera"));
    FollowCamera->SetupAttachment(CameraBoom, USpringArmComponent::SocketName); // Attach the camera to the end of the boom and let the boom adjust to match the controller orientation
    FollowCamera->bUsePawnControlRotation = false;
    
    static ConstructorHelpers::FObjectFinder<UInputMappingContext> InputMappingContextRef(
            TEXT("/Script/EnhancedInput.InputMappingContext'/Game/Siru/Input/Input.Input'")
        );
    if (InputMappingContextRef.Object != nullptr)
    {
        InputMappingContext = InputMappingContextRef.Object;
    }
    
    static ConstructorHelpers::FObjectFinder<UInputAction> InputActionAttackRef(
        TEXT("/Script/EnhancedInput.InputAction'/Game/Siru/Input/IA_Attack.IA_Attack'")
    );
    if (InputActionAttackRef.Object != nullptr)
    {
        AttackAction = InputActionAttackRef.Object;
    }

    static ConstructorHelpers::FObjectFinder<UInputAction> InputActionInteractionRef(
        TEXT("/Script/EnhancedInput.InputAction'/Game/Siru/Input/IA_Interaction.IA_Interaction'")
    );
    if (InputActionInteractionRef.Object != nullptr)
    {
        Interaction = InputActionInteractionRef.Object;
    }

    static ConstructorHelpers::FObjectFinder<UInputAction> InputActionJumpRef(
        TEXT("/Script/EnhancedInput.InputAction'/Game/Siru/Input/IA_Jump.IA_Jump'")
    );
    if (InputActionJumpRef.Object != nullptr)
    {
        JumpAction = InputActionJumpRef.Object;
    }

    static ConstructorHelpers::FObjectFinder<UInputAction> InputActionLookRef(
        TEXT("/Script/EnhancedInput.InputAction'/Game/Siru/Input/IA_Look.IA_Look'")
    );
    if (InputActionLookRef.Object != nullptr)
    {
        LookAction = InputActionLookRef.Object;
    }

    static ConstructorHelpers::FObjectFinder<UInputAction> InputActionMoveRef(
        TEXT("/Script/EnhancedInput.InputAction'/Game/Siru/Input/IA_Move.IA_Move'")
    );
    if (InputActionMoveRef.Object != nullptr)
    {
        MoveAction = InputActionMoveRef.Object;
    }

    static ConstructorHelpers::FObjectFinder<UInputAction> InputActionToggleCraftRef(
        TEXT("/Script/EnhancedInput.InputAction'/Game/Siru/Input/IA_ToggleCraft.IA_ToggleCraft'")
    );
    if (InputActionToggleCraftRef.Object != nullptr)
    {
        ToggleCraft = InputActionToggleCraftRef.Object;
    }

    static ConstructorHelpers::FObjectFinder<UInputAction> InputActionToggleInventoryRef(
        TEXT("/Script/EnhancedInput.InputAction'/Game/Siru/Input/IA_ToggleInventory.IA_ToggleInventory'")
    );
    if (InputActionToggleInventoryRef.Object != nullptr)
    {
        ToggleInventory = InputActionToggleInventoryRef.Object;
    }

    static ConstructorHelpers::FObjectFinder<UInputAction> InputActionToggleMenuRef(
        TEXT("/Script/EnhancedInput.InputAction'/Game/Siru/Input/IA_ToggleMenu.IA_ToggleMenu'")
    );
    if (InputActionToggleMenuRef.Object != nullptr)
    {
        ToggleMenu = InputActionToggleMenuRef.Object;
    }

    static ConstructorHelpers::FObjectFinder<UInputAction> InputActionToggleMissionRef(
        TEXT("/Script/EnhancedInput.InputAction'/Game/Siru/Input/IA_ToggleMission.IA_ToggleMission'")
    );
    if (InputActionToggleMissionRef.Object != nullptr)
    {
        ToggleMission = InputActionToggleMissionRef.Object;
    }

    WeaponRange1 = CreateDefaultSubobject<UBoxComponent>(TEXT("WeaponRange1"));
    WeaponRange2 = CreateDefaultSubobject<UBoxComponent>(TEXT("WeaponRange2"));
    WeaponRange3 = CreateDefaultSubobject<UBoxComponent>(TEXT("WeaponRange3"));

    WeaponRange1->SetRelativeLocation(FVector(68.929525f, 22.923540f, -18.492470f));
    WeaponRange1->SetRelativeRotation(FRotator(0.f, -180.f, 0.f));
    WeaponRange1->SetRelativeScale3D(FVector(3.04f, 0.79f, 1.79f));

    WeaponRange2->SetRelativeLocation(FVector(118.817743f, 26.265058f, -18.492470f));
    WeaponRange2->SetRelativeRotation(FRotator(0.f, -180.f, 0.f));
    WeaponRange2->SetRelativeScale3D(FVector(4.54f, 0.79f, 1.79f));

    WeaponRange3->SetRelativeLocation(FVector(69.597829f, 12.945897f, -18.492470f));
    WeaponRange3->SetRelativeRotation(FRotator(0.f, -180.f, 0.f));
    WeaponRange3->SetRelativeScale3D(FVector(2.54f, 3.54f, 1.79f));

    WeaponRange1->SetupAttachment(GetMesh());
    WeaponRange2->SetupAttachment(GetMesh());
    WeaponRange3->SetupAttachment(GetMesh());
    
}

void APlayerCharacter::BeginPlay()
{
    Super::BeginPlay();

    LOPC = Cast<ALOPlayerController>(GetController());

    if (LOPC)
    {
        if (LOPC->HUD)
        {
            InitWidgetsFromHUD();
        }
        else
        {
            LOPC->OnHUDInitialized.AddDynamic(this, &APlayerCharacter::InitWidgetsFromHUD);
        }
    }

    /*GetWorld()->GetTimerManager().SetTimer(
        SpawnTimerHandle,
        this,
        &APlayerCharacter::SpawnActorAround1,
        SpawnInterval,
        true
    );
    GetWorld()->GetTimerManager().SetTimer(
        SpawnTimerHandle,
        this,
        &APlayerCharacter::SpawnActorAround2,
        SpawnInterval,
        true
    );*/
}

void APlayerCharacter::Tick(float DeltaSeconds)
{
    Super::Tick(DeltaSeconds);
	
    if (!Sun) 
        return;
    FVector Start = GetActorLocation();
    FVector SunDirection = -Sun->GetActorForwardVector(); // 태양에서 오는 빛 방향
    FVector End = Start + SunDirection * 10000.0f; // 충분히 먼 거리

    FHitResult Hit;
    FCollisionQueryParams Params;
    Params.AddIgnoredActor(this);

    bool bHit = GetWorld()->LineTraceSingleByChannel(Hit, Start, End, ECC_Visibility, Params);

    bIsInShadow = bHit;

    // 디버그
    if (bIsInShadow)
    {
        if (!ASC->HasMatchingGameplayTag(FGameplayTag::RequestGameplayTag(FName("State.InShadow"))))
        {
            ASC->AddLooseGameplayTag(FGameplayTag::RequestGameplayTag(FName("State.InShadow")));
        }
    }
    else
    {
        if (ASC->HasMatchingGameplayTag(FGameplayTag::RequestGameplayTag(FName("State.InShadow"))))
        {
            ASC->RemoveLooseGameplayTag(FGameplayTag::RequestGameplayTag(FName("State.InShadow")));
        }
    }
}


class UAbilitySystemComponent* APlayerCharacter::GetAbilitySystemComponent() const
{
	return ASC;
}

void APlayerCharacter::FellOutOfWorld(const class UDamageType& dmgType)
{
    Super::FellOutOfWorld(dmgType);
}

void APlayerCharacter::PossessedBy(AController* NewController)
{
	Super::PossessedBy(NewController);
    Sun = Cast<ADirectionalLight>(UGameplayStatics::GetActorOfClass(GetWorld(), ADirectionalLight::StaticClass()));
    LOPC = Cast<ALOPlayerController>(NewController);
    
    if (ALOPlayerState* PS = GetPlayerState<ALOPlayerState>())
    {
        PS->InitASC();
        ASC = Cast<ULOAbilitySystemComponent>(PS->GetAbilitySystemComponent());
        ASC ->InitAbilityActorInfo(PS,this);

        for (const auto& StartAbility : StartAbilities)
        {
            FGameplayAbilitySpec StartSpec(StartAbility);
            ASC->GiveAbility(StartSpec);
        }

        for (const auto& StartInputAbilitie : StartInputAbilities)
        {
            FGameplayAbilitySpec StartSpec(StartInputAbilitie.Value);
            StartSpec.InputID = StartInputAbilitie.Key;
            ASC->GiveAbility(StartSpec);
        }
        StartReduceStat();
    }
}

void APlayerCharacter::NotifyControllerChanged()
{
    Super::NotifyControllerChanged();

    // Add Input Mapping Context
    if (APlayerController* PlayerController = Cast<APlayerController>(Controller))
    {
        if (UEnhancedInputLocalPlayerSubsystem* Subsystem = ULocalPlayer::GetSubsystem<UEnhancedInputLocalPlayerSubsystem>(PlayerController->GetLocalPlayer()))
        {
            Subsystem->AddMappingContext(InputMappingContext, 0);
        }
    }    
}

void APlayerCharacter::Move(const FInputActionValue& Value)
{
    FVector2D MovementVector = Value.Get<FVector2D>();

    if (IsValid(Controller))
    {
        const FRotator Rotation = Controller->GetControlRotation();
        const FRotator YawRotation(0, Rotation.Yaw, 0);

        const FVector ForwardDirection = FRotationMatrix(YawRotation).GetUnitAxis(EAxis::X);
        const FVector RightDirection = FRotationMatrix(YawRotation).GetUnitAxis(EAxis::Y);

        AddMovementInput(ForwardDirection, MovementVector.Y);
        AddMovementInput(RightDirection, MovementVector.X);
    }
}

void APlayerCharacter::Look(const FInputActionValue& Value)
{
    FVector2D LookAxisVector = Value.Get<FVector2D>();

    if (IsValid(Controller))
    {
        AddControllerYawInput(LookAxisVector.X);
        AddControllerPitchInput(LookAxisVector.Y);
    }
}

void APlayerCharacter::InputPressed(int32 InputID)
{
    FGameplayAbilitySpec* Spec = ASC->FindAbilitySpecFromInputID(InputID);

    if (Spec)
    {
        Spec->InputPressed = true;
        if (Spec->IsActive())
        {
            ASC->AbilitySpecInputPressed(*Spec);
        }
        else
        {
            ASC->TryActivateAbility(Spec->Handle);
        }
    }

    if ((InputID == 1 || InputID == 6) && ASC->HasMatchingGameplayTag(FGameplayTag::RequestGameplayTag("State.NearTent")))
    {
        Cast<ALOGameModeBase>(GetWorld()->GetAuthGameMode())->SetSpawnPoint(TentTransform);
    }
}

void APlayerCharacter::InputReleased(int32 InputID)
{
    FGameplayAbilitySpec* Spec = ASC->FindAbilitySpecFromInputID(InputID);

    if (Spec)
    {
        Spec->InputPressed = false;
        if (Spec->IsActive())
        {
            ASC->AbilitySpecInputReleased(*Spec);
        }
    }
}

void APlayerCharacter::ToggleCraftFunction(const FInputActionValue& Value)
{
    if (!CraftingWidget || !InventoryWidget) return;

    APlayerController* PC = Cast<APlayerController>(GetController());
    if (CraftingWidget->IsVisible())
    {
        CraftingWidget->SetVisibility(ESlateVisibility::Hidden);

        PC->SetInputMode(FInputModeGameOnly());
        PC->bShowMouseCursor = false;
    }
    else
    {
        UE_LOG(LogTemp, Warning, TEXT("ToggleCraftFunction"));
        CraftingWidget->SetVisibility(ESlateVisibility::Visible);

        if (InventoryWidget->IsVisible())
        {
            InventoryWidget->SetVisibility(ESlateVisibility::Hidden);
        }

        FInputModeGameAndUI InputMode;
        InputMode.SetWidgetToFocus(CraftingWidget->TakeWidget());
        InputMode.SetLockMouseToViewportBehavior(EMouseLockMode::DoNotLock);
        PC->SetInputMode(InputMode);
        PC->bShowMouseCursor = true;
    }
}

void APlayerCharacter::ToggleInventoryFunction(const FInputActionValue& Value)
{

    if (!CraftingWidget || !InventoryWidget) return;

    APlayerController* PC = Cast<APlayerController>(GetController());
    if (InventoryWidget->IsVisible())
    {
        InventoryWidget->SetVisibility(ESlateVisibility::Hidden);

        PC->SetInputMode(FInputModeGameOnly());
        PC->bShowMouseCursor = false;
    }
    else
    {
        UE_LOG(LogTemp, Warning, TEXT("ToggleInventoryFunction"));
        InventoryWidget->SetVisibility(ESlateVisibility::Visible);

        if (CraftingWidget->IsVisible())
        {
            CraftingWidget->SetVisibility(ESlateVisibility::Hidden);
        }

        FInputModeGameAndUI InputMode;
        InputMode.SetWidgetToFocus(InventoryWidget->TakeWidget());
        InputMode.SetLockMouseToViewportBehavior(EMouseLockMode::DoNotLock);
        PC->SetInputMode(InputMode);
        PC->bShowMouseCursor = true;
    }
}

void APlayerCharacter::InteractionFuction(const FInputActionValue& Value)
{
    //InventoryManager->GetItem()
    // Get interaction Item Data
	// InventoryManager->GetItem( Item Data );
}

void APlayerCharacter::InitWidgetsFromHUD()
{
    if (!LOPC || !LOPC->HUD) return;

    InventoryWidget = LOPC->HUD->GetInventoryWidget();
    CraftingWidget = LOPC->HUD->GetCraftingWidget();

    TempAWidget = LOPC->HUD->TempWidget;

    if (!CraftingWidget || !InventoryWidget)
    {
        FTimerHandle RetryHandle;
        GetWorld()->GetTimerManager().SetTimer(RetryHandle, [this]()
            {
                InitWidgetsFromHUD();
            }, 0.05f, false);
        return;
    }

    InventoryManager = Cast<AInventoryManager>(
        UGameplayStatics::GetActorOfClass(GetWorld(), AInventoryManager::StaticClass()));

    CraftingManager = Cast<ACraftingManager>(
        UGameplayStatics::GetActorOfClass(GetWorld(), ACraftingManager::StaticClass()));

    if (InventoryManager)
        InventoryItems = InventoryManager->ItemDataList;
}


void APlayerCharacter::SetWeaponRange(int32 Value)
{
    WeaponRange1->SetCollisionEnabled(ECollisionEnabled::NoCollision);
    WeaponRange2->SetCollisionEnabled(ECollisionEnabled::NoCollision);
    WeaponRange3->SetCollisionEnabled(ECollisionEnabled::NoCollision);
    
    switch (Value)
    {
    case 1:
        WeaponRange1->SetCollisionEnabled(ECollisionEnabled::QueryOnly);
        break;
    case 2:
        WeaponRange3->SetCollisionEnabled(ECollisionEnabled::QueryOnly);
        break;
    case 3:
        WeaponRange2->SetCollisionEnabled(ECollisionEnabled::QueryOnly);
        break;
    default:
        break;
    }
}

void APlayerCharacter::EndHitCheck()
{
    if (const ULOAttributeSet* MyAttrSet = Cast<ULOAttributeSet>(ASC->GetAttributeSet(ULOAttributeSet::StaticClass())))
    {
        float Damage = MyAttrSet->GetDamage();
        for (AEnemyCh* Enemy : HitList)
        {
            Enemy->DecreaseHP(Damage);
            UE_LOG(LogTemp,Log,TEXT("Damage : %f"),Damage);
        }
    }
    HitList.Empty();
}

void APlayerCharacter::ToggleMissionFunction()
{
    LOPC->HUD->OpenGoal();
}

void APlayerCharacter::SpawnActorAround1()
{
    if (!Enemy1 && !Enemy2) return;

	if (CurEnemyNum >= MaxEnemyNum) return;

    TSubclassOf<AActor> SelectedClass = Enemy1;

    FVector Origin = GetActorLocation();
    FVector RandomOffset = UKismetMathLibrary::RandomUnitVector() * FMath::FRandRange(MinRadius, MaxRadius);
    FVector SpawnLoc = Origin + RandomOffset + FVector(0, 0, SpawnHeight);


    FHitResult Hit;
    FCollisionQueryParams Params;
    Params.AddIgnoredActor(this);
    bool bHit = GetWorld()->LineTraceSingleByChannel(
        Hit,
        SpawnLoc,
        FVector(SpawnLoc.X, SpawnLoc.Y, SpawnLoc.Z - 20000.0f),
        ECC_WorldStatic,
        Params
    );

    if (bHit)
    {
        FActorSpawnParameters SpawnParams;
        SpawnParams.SpawnCollisionHandlingOverride = ESpawnActorCollisionHandlingMethod::AdjustIfPossibleButAlwaysSpawn;

        AEnemyCh* Enemy = Cast<AEnemyCh>(GetWorld()->SpawnActor<AActor>(SelectedClass, Hit.Location, FRotator::ZeroRotator, SpawnParams));

        if (AAIController* AICont = Cast<AAIController>(GetController()))
        {
            if (BehaviorTree1)
            {
                AICont->RunBehaviorTree(BehaviorTree1);
            }
        }
		CurEnemyNum++;
    }
}

void APlayerCharacter::SpawnActorAround2()
{
    if (!Enemy1 && !Enemy2) return;

    if (CurEnemyNum >= MaxEnemyNum) return;

    TSubclassOf<AActor> SelectedClass = Enemy2;

    FVector Origin = GetActorLocation();
    FVector RandomOffset = UKismetMathLibrary::RandomUnitVector() * FMath::FRandRange(MinRadius, MaxRadius);
    FVector SpawnLoc = Origin + RandomOffset + FVector(0, 0, SpawnHeight);


    FHitResult Hit;
    FCollisionQueryParams Params;
    Params.AddIgnoredActor(this);
    bool bHit = GetWorld()->LineTraceSingleByChannel(
        Hit,
        SpawnLoc,
        FVector(SpawnLoc.X, SpawnLoc.Y, SpawnLoc.Z - 20000.0f),
        ECC_WorldStatic,
        Params
    );

    if (bHit)
    {
        FActorSpawnParameters SpawnParams;
        SpawnParams.SpawnCollisionHandlingOverride = ESpawnActorCollisionHandlingMethod::AdjustIfPossibleButAlwaysSpawn;

        AEnemyCh* Enemy = Cast<AEnemyCh>(GetWorld()->SpawnActor<AActor>(SelectedClass, Hit.Location + FVector(0, 0, 100), FRotator::ZeroRotator, SpawnParams));

        if (AAIController* AICont = Cast<AAIController>(GetController()))
        {
            if (BehaviorTree2)
            {
                AICont->RunBehaviorTree(BehaviorTree2);
            }
        }
        CurEnemyNum++;
    }
}

void APlayerCharacter::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);
    UEnhancedInputComponent* EnhancedInputComponent = CastChecked<UEnhancedInputComponent>(PlayerInputComponent);

    EnhancedInputComponent->BindAction(JumpAction, ETriggerEvent::Triggered, this, &APlayerCharacter::InputPressed, 0);
    EnhancedInputComponent->BindAction(JumpAction, ETriggerEvent::Completed, this, &APlayerCharacter::InputReleased, 0);
    
    EnhancedInputComponent->BindAction(MoveAction, ETriggerEvent::Triggered, this, &APlayerCharacter::Move);
    EnhancedInputComponent->BindAction(LookAction, ETriggerEvent::Triggered, this, &APlayerCharacter::Look);
    EnhancedInputComponent->BindAction(AttackAction, ETriggerEvent::Triggered, this, &APlayerCharacter::InputPressed, 1);

    EnhancedInputComponent->BindAction(Interaction, ETriggerEvent::Triggered, this, &APlayerCharacter::InputPressed, 6);
    EnhancedInputComponent->BindAction(ToggleCraft, ETriggerEvent::Triggered, this, &APlayerCharacter::ToggleCraftFunction);
    EnhancedInputComponent->BindAction(ToggleInventory, ETriggerEvent::Triggered, this, &APlayerCharacter::ToggleInventoryFunction);
    EnhancedInputComponent->BindAction(ToggleMission, ETriggerEvent::Triggered, this, &APlayerCharacter::ToggleMissionFunction);    
}


void APlayerCharacter::SettingWidget()
{
    UE_LOG(LogTemp, Warning, TEXT("TempAWidget"));
    if (TempAWidget->IsVisible())
    {
        TempAWidget->SetVisibility(ESlateVisibility::Hidden);
    }
    else 
    {
        TempAWidget->SetVisibility(ESlateVisibility::Visible);
    }

}