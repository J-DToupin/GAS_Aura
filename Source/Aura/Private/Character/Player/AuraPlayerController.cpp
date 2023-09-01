// Copyright JDgame


#include "Character/Player/AuraPlayerController.h"

#include "EnhancedInputSubsystems.h"
#include "EnhancedInputComponent.h"
#include "Interaction/TargetInterface.h"

AAuraPlayerController::AAuraPlayerController()
{
	bReplicates = true;
}

void AAuraPlayerController::PlayerTick(float DeltaTime)
{
	Super::PlayerTick(DeltaTime);

	CursorTrace();
	
}

void AAuraPlayerController::BeginPlay()
{
	Super::BeginPlay();

	// regarde si le pointer nest pas null 
	check(AuraContext);

	UEnhancedInputLocalPlayerSubsystem* Subsystem = ULocalPlayer::GetSubsystem<UEnhancedInputLocalPlayerSubsystem>(GetLocalPlayer());
	check(Subsystem);

	// rajouter des mappingContext posibiliter dajouter plusieur avec un system de priorite
	Subsystem->AddMappingContext(AuraContext, 0);


	// set the mouse
	bShowMouseCursor = true;
	DefaultMouseCursor = EMouseCursor::Hand;
	
	FInputModeGameAndUI InputModeData;
	InputModeData.SetLockMouseToViewportBehavior(EMouseLockMode::DoNotLock);
	InputModeData.SetHideCursorDuringCapture(false);
	SetInputMode(InputModeData);
}

void AAuraPlayerController::SetupInputComponent()
{
	Super::SetupInputComponent();

	UEnhancedInputComponent* EnhancedInputComponent = CastChecked<UEnhancedInputComponent>(InputComponent);

	EnhancedInputComponent->BindAction(MoveAction, ETriggerEvent::Triggered, this, &AAuraPlayerController::Move);
	
}

void AAuraPlayerController::Move(const FInputActionValue& InputActionValue)
{
	const FRotator YawRotation{0.f, GetControlRotation().Yaw, 0.f};

	const FVector ForwardDirection = FRotationMatrix(YawRotation).GetUnitAxis(EAxis::X);
	const FVector RightDirection = FRotationMatrix(YawRotation).GetUnitAxis(EAxis::Y);

	if (APawn* ControlledPawn = GetPawn<APawn>())
	{
		ControlledPawn->AddMovementInput(ForwardDirection, InputActionValue[1]);
		ControlledPawn->AddMovementInput(RightDirection, InputActionValue[0]);
	}
	
}

inline void AAuraPlayerController::CursorTrace()
{
	FHitResult CursorHit;

	GetHitResultUnderCursor(ECC_Visibility, false, CursorHit);

	if (!CursorHit.bBlockingHit) return;
	
	if (CursorHit.GetActor()->Implements<UTargetInterface>())
	{
		LastActor = ThisActor;
		ThisActor = CursorHit.GetActor();

		if (LastActor == nullptr)
		{
			// Case B
			ITargetInterface::Execute_HighLightActor(ThisActor);
		}

		else if (ThisActor != LastActor)
		{
			// Case D
			ITargetInterface::Execute_HighLightActor(ThisActor);
			ITargetInterface::Execute_UnHighLightActor(LastActor);
		}

		// Case E
		return;
	}

	if (LastActor)
	{
		// Case C
		ITargetInterface::Execute_UnHighLightActor(LastActor);
		ThisActor = nullptr;
		LastActor = nullptr;
	}

	// Case A
}

