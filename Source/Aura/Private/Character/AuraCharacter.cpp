// Copyright JDgame


#include "Character/AuraCharacter.h"

#include "AbilitySystemComponent.h"
#include "NiagaraComponent.h"
#include "Camera/CameraComponent.h"
#include "Character/Player/AuraPlayerController.h"
#include "Character/Player/AuraPlayerState.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "GameFramework/SpringArmComponent.h"
#include "GAS/AuraAbilitySystemComponent.h"
#include "GAS/Data/AuraLevelUpInfo.h"
#include "UI/AuraHUD.h"


// Sets default values
AAuraCharacter::AAuraCharacter()
{
	LevelUpNiagaraComponent = CreateDefaultSubobject<UNiagaraComponent>(TEXT("LevelUpNiagara"));
	LevelUpNiagaraComponent->SetupAttachment(GetRootComponent());
	LevelUpNiagaraComponent->bAutoActivate = false;
	
	GetCharacterMovement()->bOrientRotationToMovement = true;
	GetCharacterMovement()->RotationRate = FRotator(0.f, 400.f, 0.f);

	//option for TopDown game
	GetCharacterMovement()->bConstrainToPlane = true;
	GetCharacterMovement()->bSnapToPlaneAtStart = true;


	bUseControllerRotationPitch = false;
	bUseControllerRotationRoll = false;
	bUseControllerRotationYaw = false;
	
	//Setup Component
	SpringArmComponent = CreateDefaultSubobject<USpringArmComponent>("SpringArm");
	SpringArmComponent->SetupAttachment(GetRootComponent());
	SpringArmComponent->SetRelativeRotation(FRotator(-45.f, 0.f, 0.f));
	SpringArmComponent->bEnableCameraLag = true;
	SpringArmComponent->bInheritPitch = false;
	SpringArmComponent->bInheritRoll = false;
	SpringArmComponent->bInheritYaw = false;
	SpringArmComponent->TargetArmLength = 800.f;
	
	CameraComponent = CreateDefaultSubobject<UCameraComponent>("Camera");
	CameraComponent->SetupAttachment(SpringArmComponent);
	CameraComponent->bUsePawnControlRotation = false;
}

void AAuraCharacter::PossessedBy(AController* NewController)
{
	Super::PossessedBy(NewController);

	InitAbilityActorInfo();
	AddCharacterAbilities();
}

void AAuraCharacter::OnRep_PlayerState()
{
	Super::OnRep_PlayerState();
	
	InitAbilityActorInfo();
}

int32 AAuraCharacter::GetCharacterLevel_Implementation()
{
	check(AuraPlayerState);
	
	return AuraPlayerState->GetCharacterLevel();
}

FVector AAuraCharacter::GetMouseLocation()
{
	FVector MousePosition{};
	if (const APlayerController* PlayerController = Cast<APlayerController>(GetController()))
	{
		PlayerController->GetMousePosition(MousePosition.X, MousePosition.Y);
	}
	return MousePosition;
}

void AAuraCharacter::AddToXp_Implementation(int32 InXp)
{
	check(AuraPlayerState);

	AuraPlayerState->AddToXP(InXp);
}

void AAuraCharacter::AddToPlayerLevel_Implementation(int32 NumLevel)
{
	check(AuraPlayerState);
	AuraPlayerState->AddToLevel(NumLevel);
}

void AAuraCharacter::AddToAttributePoint_Implementation(int32 NumAttributePoint)
{
	check(AuraPlayerState);
	AuraPlayerState->AddToAttributePoint(NumAttributePoint);
}

void AAuraCharacter::AddToSpellPoint_Implementation(int32 NumSpellPoint)
{
	check(AuraPlayerState);
	AuraPlayerState->AddToSpellPoint(NumSpellPoint);
}

int32 AAuraCharacter::GetAttributePointsReward_Implementation(const int32 Level) const
{
	check(AuraPlayerState);

	return AuraPlayerState->LevelUpInfo->LevelUpInfos[Level].AttributePointReward;
}

int32 AAuraCharacter::GetAttributePoints_Implementation() const
{
	check(AuraPlayerState);
	return AuraPlayerState->GetAttributePoint();
}

int32 AAuraCharacter::GetSpellPoints_Implementation() const
{
	check(AuraPlayerState);
	return AuraPlayerState->GetSpellPoint();
}

int32 AAuraCharacter::GetSpellPointsReward_Implementation(const int32 Level) const
{
	check(AuraPlayerState);

	return AuraPlayerState->LevelUpInfo->LevelUpInfos[Level].SpellPointReward;
}

int32 AAuraCharacter::GetCurrentXp_Implementation() const
{
	check(AuraPlayerState);

	return AuraPlayerState->GetCharacterExperience();
}

const int32 AAuraCharacter::GetLevelForXp_Implementation(const int32 TotalXp)
{
	check(AuraPlayerState);
	
	return AuraPlayerState->LevelUpInfo->FindLevelByXp(TotalXp);
}



void AAuraCharacter::LevelUp_Implementation()
{
	MulticastLevelUpParticles();
}


// Called when the game starts or when spawned
void AAuraCharacter::BeginPlay()
{
	Super::BeginPlay();
	
}

void AAuraCharacter::InitAbilityActorInfo()
{
	AuraPlayerState = GetPlayerState<AAuraPlayerState>();
	check(AuraPlayerState);

	AuraPlayerState->GetAbilitySystemComponent()->InitAbilityActorInfo(AuraPlayerState, this);

	Cast<UAuraAbilitySystemComponent>(AuraPlayerState->GetAbilitySystemComponent())->AbilityActorInfoSet();

	AbilitySystemComponent = AuraPlayerState->GetAbilitySystemComponent();
	AttributeSet = AuraPlayerState->GetAttributeSet();
	OnAscRegisteredDelegate.Broadcast(AbilitySystemComponent);

	if (AAuraPlayerController* AuraPlayerController = GetController<AAuraPlayerController>())
	{
		if (AAuraHUD* AuraHUD = AuraPlayerController->GetHUD<AAuraHUD>() )
		{
			AuraHUD->InitOverlay(AuraPlayerController, AuraPlayerState, AbilitySystemComponent, AttributeSet);
		}
	}
	InitializeDefaultAttributes();
}

void AAuraCharacter::MulticastLevelUpParticles_Implementation() const
{
	if (IsValid(LevelUpNiagaraComponent))
	{
		const FRotator CameraRotation = CameraComponent->GetComponentRotation();
		LevelUpNiagaraComponent->SetWorldRotation(CameraRotation);
		LevelUpNiagaraComponent->Activate(true);
	}
}
