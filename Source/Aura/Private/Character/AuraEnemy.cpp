// Copyright JDgame


#include "Character/AuraEnemy.h"

#include "AbilitySystemComponent.h"
#include "GAS/AuraAbilitySystemComponent.h"
#include "GAS/AuraAttributeSet.h"


// Sets default values
AAuraEnemy::AAuraEnemy()
{
	GetMesh()->SetCollisionResponseToChannel(ECC_Visibility, ECR_Block);

	AbilitySystemComponent = CreateDefaultSubobject<UAuraAbilitySystemComponent>("AbilityComponent");
	AbilitySystemComponent->SetIsReplicated(true);
	AbilitySystemComponent->SetReplicationMode(EGameplayEffectReplicationMode::Minimal);
	
	AttributeSet = CreateDefaultSubobject<UAuraAttributeSet>("AttributeSet");
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

// Called when the game starts or when spawned
void AAuraEnemy::BeginPlay()
{
	Super::BeginPlay();
	InitAbilityActorInfo();
	
}

void AAuraEnemy::InitAbilityActorInfo()
{
	
	AbilitySystemComponent->InitAbilityActorInfo(this, this);
	Cast<UAuraAbilitySystemComponent>(AbilitySystemComponent)->AbilityActorInfoSet();
	
}
