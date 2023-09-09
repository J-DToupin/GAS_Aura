// Copyright JDgame

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "ActiveGameplayEffectHandle.h"
#include "AuraEffectActor.generated.h"

class UAbilitySystemComponent;
struct FActiveGameplayEffectHandle;
class UGameplayEffect;
class USphereComponent;

UENUM(BlueprintType)
enum class EEffectApplicationPolicy
{
	ApplyOnOverlap,
	ApplyOnEndOverlap,
	DoNotApply
};

UENUM(BlueprintType)
enum class EEffectRemovalPolicy
{
	RemoveOnOverlap,
	RemoveEndOverlap,
	DoNotRemove
};

UCLASS()
class AURA_API AAuraEffectActor : public AActor
{
	GENERATED_BODY()

public:
	// Sets default values for this actor's properties
	AAuraEffectActor();

	UFUNCTION()
	virtual void OnOverlap(UPrimitiveComponent* OverlappedComponent,
		AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex,
		bool bFromSweep, const FHitResult & SweepResult);

	UFUNCTION()
	virtual void EndOverLap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor,
		UPrimitiveComponent* OtherComp, int32 OtherBodyIndex);


protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

	UFUNCTION(BlueprintCallable)
	void ApplyEffectToInstigator(AActor* InstigatorActor, TSubclassOf<UGameplayEffect> GEClass);

	UFUNCTION(BlueprintCallable)
	void RemoveEffectToInstigator(AActor* InstigatorActor);

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category="Applied Effet")
	EEffectApplicationPolicy EffectApplicationPolicy =  EEffectApplicationPolicy::ApplyOnOverlap;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category="Applied Effet")
	EEffectRemovalPolicy EffectRemovalPolicy = EEffectRemovalPolicy::DoNotRemove;
	
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category="Applied Effet")
	TSubclassOf<UGameplayEffect> GamePlayEffectClass;

	TMap<UAbilitySystemComponent*, FActiveGameplayEffectHandle> ActiveEffectHandles; 

private:

	
};
