// Copyright JDgame


#include "Character/AuraEnemy.h"

#include "AbilitySystemComponent.h"
#include "Components/WidgetComponent.h"
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
	
	AttributeSet = CreateDefaultSubobject<UAuraAttributeSet>("AttributeSet");

	HealthBar = CreateDefaultSubobject<UWidgetComponent>("HealthBar");
	HealthBar->SetupAttachment(GetRootComponent());
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

int32 AAuraEnemy::GetCharacterLevel()
{
	return CharacterLevel;
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
	}
}

// Called when the game starts or when spawned
void AAuraEnemy::BeginPlay()
{
	Super::BeginPlay();
	InitAbilityActorInfo();

	if (UAuraUserWidget* AuraUserWidget = Cast<UAuraUserWidget>(HealthBar->GetUserWidgetObject()))
	{
		AuraUserWidget->SetWidgetController(this);
	}
	BroadcastInitialValues();
	BindCallbacksToDependencies();
}

void AAuraEnemy::InitAbilityActorInfo()
{
	
	AbilitySystemComponent->InitAbilityActorInfo(this, this);
	Cast<UAuraAbilitySystemComponent>(AbilitySystemComponent)->AbilityActorInfoSet();

	InitializeDefaultAttributes();
	
}

void AAuraEnemy::InitializeDefaultAttributes() const
{
	UAuraAbilitySystemLibrary::InitializeDefaultAttributes(this, CharacterClass, CharacterLevel, AbilitySystemComponent);
}
