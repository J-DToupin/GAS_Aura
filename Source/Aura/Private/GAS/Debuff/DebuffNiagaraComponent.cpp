// Copyright JDgame


#include "GAS/Debuff/DebuffNiagaraComponent.h"

#include "AbilitySystemBlueprintLibrary.h"
#include "AbilitySystemComponent.h"
#include "Interaction/CombatInterface.h"


// Sets default values for this component's properties
UDebuffNiagaraComponent::UDebuffNiagaraComponent()
{
	bAutoActivate = false;
	
}


void UDebuffNiagaraComponent::BeginPlay()
{
	Super::BeginPlay();

	if (ICombatInterface* CombatInterface = Cast<ICombatInterface>(GetOwner()))
	{
		CombatInterface->GetOnDeathDelegate().AddDynamic(this, &ThisClass::UDebuffNiagaraComponent::OnOwnerDeath);
		
		UAbilitySystemComponent* OwnerAbilitySystemComponent = UAbilitySystemBlueprintLibrary::GetAbilitySystemComponent(GetOwner());

		if (OwnerAbilitySystemComponent)
		{
			OwnerAbilitySystemComponent->RegisterGameplayTagEvent(DebuffTag, EGameplayTagEventType::NewOrRemoved).
			AddUObject(this, &UDebuffNiagaraComponent::DebuffTagChanged);
		}
		else
		{
			// WeakLambda ne garde pas de reference de l'object il vas pouvoir etre detruit
			CombatInterface->GetOnASCRegisteredDelegate().AddWeakLambda(this, [this](UAbilitySystemComponent* InASC)
			{
				InASC->RegisterGameplayTagEvent(DebuffTag, EGameplayTagEventType::NewOrRemoved).
				AddUObject(this, &UDebuffNiagaraComponent::DebuffTagChanged);
			});
		}
	}	
	
}

void UDebuffNiagaraComponent::DebuffTagChanged(const FGameplayTag CallbackTag, int32 NewCount)
{
	if (NewCount > 0)
	{
		Activate();
	}
	else
	{
		Deactivate();
	}
}

void UDebuffNiagaraComponent::OnOwnerDeath(AActor* DeadActor)
{
	Deactivate();
}
