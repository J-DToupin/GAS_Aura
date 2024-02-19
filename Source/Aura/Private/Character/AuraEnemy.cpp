// Copyright JDgame


#include "Character/AuraEnemy.h"

#include "AbilitySystemComponent.h"
#include "AuraGameplayTags.h"
#include "AI/AuraAIController.h"
#include "BehaviorTree/BehaviorTree.h"
#include "BehaviorTree/BlackboardComponent.h"
#include "Components/WidgetComponent.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "GAS/AuraAbilitySystemComponent.h"
#include "GAS/AuraAbilitySystemLibrary.h"
#include "GAS/AuraAttributeSet.h"
#include "UI/Widget/AuraUserWidget.h"


// Sets default values
AAuraEnemy::AAuraEnemy()
{
	GetMesh()->SetCollisionResponseToChannel(ECC_Visibility, ECR_Block);

	AbilitySystemComponent = CreateDefaultSubobject<UAuraAbilitySystemComponent>("AbilityComponent");
	AbilitySystemComponent->SetIsReplicated(true);
	AbilitySystemComponent->SetReplicationMode(EGameplayEffectReplicationMode::Minimal);

	// Smooth rotation
	bUseControllerRotationPitch = false;
	bUseControllerRotationRoll = false;
	bUseControllerRotationYaw = false;
	GetCharacterMovement()->bUseControllerDesiredRotation = true;
	
	AttributeSet = CreateDefaultSubobject<UAuraAttributeSet>("AttributeSet");

	HealthBar = CreateDefaultSubobject<UWidgetComponent>("HealthBar");
	HealthBar->SetupAttachment(GetRootComponent());

	GetCharacterMovement()->MaxWalkSpeed = BaseWalkSpeed;
}

void AAuraEnemy::PossessedBy(AController* NewController)
{
	Super::PossessedBy(NewController);

	if (!HasAuthority()) return;
	
	AuraAIController = Cast<AAuraAIController>(NewController);
	AuraAIController->GetBlackboardComponent()->InitializeBlackboard(*BehaviorTree->BlackboardAsset);
	AuraAIController->RunBehaviorTree(BehaviorTree);
	AuraAIController->GetBlackboardComponent()->SetValueAsBool(FName("HitReacting"), false);
	AuraAIController->GetBlackboardComponent()->SetValueAsBool(FName("RangedAttacker"), CharacterClass != ECharacterClass::Warrior);
}

void AAuraEnemy::HighLightActor_Implementation()
{
	ITargetInterface::HighLightActor_Implementation();

	GetMesh()->SetRenderCustomDepth(true);
	Weapon->SetRenderCustomDepth(true);
	//UE_LOG(LogTemp, Warning, TEXT("HighLightActor %s"), *GetName());
	
}

void AAuraEnemy::UnHighLightActor_Implementation()
{
	ITargetInterface::UnHighLightActor_Implementation();

	GetMesh()->SetRenderCustomDepth(false);
	Weapon->SetRenderCustomDepth(false);
	//UE_LOG(LogTemp, Warning, TEXT("UnHighLight %s"), *GetName());

}

int32 AAuraEnemy::GetCharacterLevel_Implementation()
{
	return CharacterLevel;
}

ECharacterClass AAuraEnemy::GetCharacterClass_Implementation()
{
	return CharacterClass;
}

void AAuraEnemy::Die()
{
	SetLifeSpan(LifeSpan);
	if (AuraAIController)
	{
		AuraAIController->GetBlackboardComponent()->SetValueAsBool(FName("Dead"), true);
	}
	Super::Die();
}

void AAuraEnemy::SetCombatTarget_Implementation(AActor* InCombatTarget)
{
	CombatTarget = InCombatTarget;
}

AActor* AAuraEnemy::GetCombatTarget_Implementation() const
{
	return CombatTarget;
}

void AAuraEnemy::BroadcastInitialValues()
{
	if (const UAuraAttributeSet* AuraAttributeSet = Cast<UAuraAttributeSet>(AttributeSet))
	{
		OnHealthChangeDelegate.Broadcast(AuraAttributeSet->GetHealth());
		OnMaxHealthChangeDelegate.Broadcast(AuraAttributeSet->GetMaxHealth());
		
		OnManaChangeDelegate.Broadcast(AuraAttributeSet->GetMana());
		OnMaxManaChangeDelegate.Broadcast(AuraAttributeSet->GetMaxMana());
	}
}

void AAuraEnemy::BindCallbacksToDependencies()
{
	if (const UAuraAttributeSet* AuraAttributeSet = Cast<UAuraAttributeSet>(AttributeSet))
	{
		AbilitySystemComponent->GetGameplayAttributeValueChangeDelegate(
			AuraAttributeSet->GetHealthAttribute()).AddLambda(
				[this](const FOnAttributeChangeData& Data)
			{
				OnHealthChangeDelegate.Broadcast(Data.NewValue);
			});

		AbilitySystemComponent->GetGameplayAttributeValueChangeDelegate(
		AuraAttributeSet->GetMaxHealthAttribute()).AddLambda(
			[this](const FOnAttributeChangeData& Data)
		{
			OnMaxHealthChangeDelegate.Broadcast(Data.NewValue);
		});

		AbilitySystemComponent->GetGameplayAttributeValueChangeDelegate(
		AuraAttributeSet->GetManaAttribute()).AddLambda(
			[this](const FOnAttributeChangeData& Data)
		{
			OnManaChangeDelegate.Broadcast(Data.NewValue);
		});
		
		AbilitySystemComponent->GetGameplayAttributeValueChangeDelegate(
		AuraAttributeSet->GetMaxManaAttribute()).AddLambda(
			[this](const FOnAttributeChangeData& Data)
		{
			OnMaxManaChangeDelegate.Broadcast(Data.NewValue);
		});

		AbilitySystemComponent->RegisterGameplayTagEvent(FAuraGameplayTags::Get().Effects_HitReact).AddUObject(this, &AAuraEnemy::HitReactTagChanged);
	}
}

// Called when the game starts or when spawned
void AAuraEnemy::BeginPlay()
{
	Super::BeginPlay();

	GetCharacterMovement()->MaxWalkSpeed =  BaseWalkSpeed;
	InitAbilityActorInfo();

	if (HasAuthority())
	{
		UAuraAbilitySystemLibrary::GiveStartupAbilities(this, GetAbilitySystemComponent(), CharacterClass);
	}

	if (UAuraUserWidget* AuraUserWidget = Cast<UAuraUserWidget>(HealthBar->GetUserWidgetObject()))
	{
		AuraUserWidget->SetWidgetController(this);
	}
	BroadcastInitialValues();
	BindCallbacksToDependencies();

	AbilitySystemComponent->RegisterGameplayTagEvent(FAuraGameplayTags::Get().Effects_HitReact).AddUObject(this, &AAuraEnemy::HitReact);
}

void AAuraEnemy::HitReactTagChanged(const FGameplayTag CallbackTag, int32 NewCount)
{
	bHitReacting = NewCount > 0;
	GetCharacterMovement()->MaxWalkSpeed = bHitReacting ? 0.f : BaseWalkSpeed;
	if (AuraAIController && AuraAIController->GetBlackboardComponent())
	{
		AuraAIController->GetBlackboardComponent()->SetValueAsBool(FName("HitReacting"), bHitReacting);
	}
}

void AAuraEnemy::InitAbilityActorInfo()
{
	
	AbilitySystemComponent->InitAbilityActorInfo(this, this);
	Cast<UAuraAbilitySystemComponent>(AbilitySystemComponent)->AbilityActorInfoSet();

	if (HasAuthority())
	{
		InitializeDefaultAttributes();
	}
	OnAscRegisteredDelegate.Broadcast(AbilitySystemComponent);
}

void AAuraEnemy::InitializeDefaultAttributes() const
{
	UAuraAbilitySystemLibrary::InitializeDefaultAttributes(this, CharacterClass, CharacterLevel, AbilitySystemComponent);
}

void AAuraEnemy::HitReact(const FGameplayTag CallbackTag, int32 NewCount)
{
	bHitReacting = NewCount > 0;

	GetCharacterMovement()->MaxWalkSpeed = bHitReacting ? 0.f : BaseWalkSpeed;
}
