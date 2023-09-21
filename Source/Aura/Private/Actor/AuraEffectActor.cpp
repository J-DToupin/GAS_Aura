// Copyright JDgame


#include "Actor/AuraEffectActor.h"

#include "AbilitySystemBlueprintLibrary.h"
#include "GAS/AuraAbilitySystemComponent.h"


// Sets default values
AAuraEffectActor::AAuraEffectActor()
{

	SetRootComponent(CreateDefaultSubobject<USceneComponent>("SceneRoot"));
	
}

void AAuraEffectActor::OnOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor,
	UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{
	if (!OtherActor) return;
	
	if (EffectApplicationPolicy == EEffectApplicationPolicy::ApplyOnOverlap)
	{
		ApplyEffectToInstigator(OtherActor, GamePlayEffectClass);
	}

	if (EffectRemovalPolicy == EEffectRemovalPolicy::RemoveOnOverlap)
	{
		RemoveEffectToInstigator(OtherActor);
	}
}

void AAuraEffectActor::EndOverLap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor,
	UPrimitiveComponent* OtherComp, int32 OtherBodyIndex)
{
	if (!OtherActor) return;
	
	if (EffectApplicationPolicy == EEffectApplicationPolicy::ApplyOnEndOverlap)
	{
		ApplyEffectToInstigator(OtherActor, GamePlayEffectClass);
	}

	if (EffectRemovalPolicy == EEffectRemovalPolicy::RemoveEndOverlap)
	{
		RemoveEffectToInstigator(OtherActor);
	}
}


// Called when the game starts or when spawned
void AAuraEffectActor::BeginPlay()
{
	Super::BeginPlay();
	
}

void AAuraEffectActor::ApplyEffectToInstigator(AActor* InstigatorActor, TSubclassOf<UGameplayEffect> GEClass)
{

	check(GEClass)
	
	UAbilitySystemComponent* InstigatorActorASC = UAbilitySystemBlueprintLibrary::GetAbilitySystemComponent(InstigatorActor);

	if (InstigatorActorASC)
	{
		FGameplayEffectContextHandle EffectContextHandle = InstigatorActorASC->MakeEffectContext();
		// indique objet de la cause de l'effet
		EffectContextHandle.AddSourceObject(this);
	
		const FGameplayEffectSpecHandle EffectSpecHandle =  InstigatorActorASC->MakeOutgoingSpec(GEClass, ActorLevel, EffectContextHandle);

		const FActiveGameplayEffectHandle ActiveGameplayEffectHandle = InstigatorActorASC->ApplyGameplayEffectSpecToSelf(*EffectSpecHandle.Data.Get());

		if (EffectSpecHandle.Data.Get()->Def.Get()->DurationPolicy == EGameplayEffectDurationType::Infinite
			&& EffectRemovalPolicy != EEffectRemovalPolicy::DoNotRemove)
		{
			ActiveEffectHandles.Add(InstigatorActorASC, ActiveGameplayEffectHandle);
		}
		
	}
}

void AAuraEffectActor::RemoveEffectToInstigator(AActor* InstigatorActor)
{
	
	UAbilitySystemComponent* InstigatorASC = UAbilitySystemBlueprintLibrary::GetAbilitySystemComponent(InstigatorActor);

	if (!IsValid(InstigatorASC)) return;
	
	if (ActiveEffectHandles.Contains(InstigatorASC))
	{
		// -1 == remove tout les stack 
		InstigatorASC->RemoveActiveGameplayEffect(ActiveEffectHandles.FindAndRemoveChecked(InstigatorASC), 1);
	}
}
